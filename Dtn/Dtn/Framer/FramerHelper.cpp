//!
//! @file TODO
//! @brief Reads lines from stdin and sends these lines in bundles.
//!     Receives bundles and writes them to stdout
//!

#include <cstdio>
#include <Fw/Types/BasicTypes.hpp>
#include "ltpP.h"
#include "FramerHelper.hpp"

namespace Dtn
{

FramerHelper::FramerHelper
(
    char *_ownEid,
    char *_destEid,
    U64 _remoteEngineId,
    BufferAllocateFunc _bufferAllocate,
    BufferOutFunc _bufferOut
) :
    ownEid(_ownEid),
    destEid(_destEid),
    remoteEngineId(_remoteEngineId),
    bufferAllocate(_bufferAllocate),
    bufferOut(_bufferOut)
{ }

void FramerHelper::configure(pthread_mutex_t& _sdrMutex)
{
    sdrMutex = &_sdrMutex;

    if (bp_attach() < 0)
    {
        printf("[FramerHelper] Can't bp_attach()\n");
    }

    if (bp_open(ownEid, &sap) < 0)
    {
        printf("[FramerHelper] Can't open own endpoint\n");
    }

    if (pthread_mutex_lock(&(*sdrMutex)) != 0)
    {
        putErrmsg("Couldn't take sdr mutex.", NULL);
        return;
    }

    bpSdr = bp_get_sdr();

    pthread_mutex_unlock(&(*sdrMutex));

    // TODO on exit run:
    // bp_close(sap); // TODO or bp_interrupt() ?
    // bp_detach();
}

void *FramerHelper::ltpFrameWrapper(void *self)
{
    // TODO have had issues with member variables (like `remoteEngineId`)
    // not being preserved. If things fail check here:
    FramerHelper& instance = *static_cast<FramerHelper *>(self);
    instance.ltpFrame();
    return nullptr;
}

void FramerHelper::ltpFrame()
{
    if (ltpInit(0) < 0)
    {
        printf("[FramerHelper] LTP: Error initializing LTP\n");
    }

    LtpVspan *vspan;
    PsmAddress vspanElt; // Is an unsigned long

    if (pthread_mutex_lock(&(*sdrMutex)) != 0)
    {
        putErrmsg("Couldn't take sdr mutex.", NULL);
        return;
    }

    Sdr sdr = getIonsdr();

    if (!sdr_begin_xn(sdr)) // Needed to get "lock" (see `udplso.c`)
    {
        pthread_mutex_unlock(&(*sdrMutex));
        printf("[FramerHelper] LTP: Error starting SDR transaction\n");
        return; // TODO clean up this return logic
    }

    findSpan(remoteEngineId, &vspan, &vspanElt);
    if (vspanElt == 0)
    {
        sdr_exit_xn(sdr);
        pthread_mutex_unlock(&(*sdrMutex));
        printf("[FramerHelper] LTP: No such engine in database: %llu\n", remoteEngineId);
        return; // TODO clean up this return logic
    }
    printf("[FramerHelper] LTP: Found span: %lu\n", vspanElt);

    sdr_exit_xn(sdr); // "Exit" is okay here as we don't want to persist any changes to SDR DB
    pthread_mutex_unlock(&(*sdrMutex));

    for (;;)
    {
        if (sm_SemEnded(vspan->segSemaphore))
        {
            printf("[ltp-test] LTP: Semaphore ended prematurely for vspan");
            return; // TODO how should this error be handled?
        }

        char *segment;
        int segmentLen = ltpDequeueOutboundSegment(vspan, &segment);
        if (segmentLen <= 0)
        {
            printf("[FramerHelper] LTP: Bad segment length: %d\n", segmentLen);
            return; // TODO how should this error be handled?
        }
        //printf("[FramerHelper] LTP: Dequeued %d bytes\n", segmentLen);

        //printf("[FramerHelper] LTP: Hex dump start\n");
        //for (int i = 0; i < segmentLen; i++)
        //{
        //    printf("%02x", (unsigned char)segment[i]);
        //}
        //printf("\n");
        //printf("[FramerHelper] LTP: Hex dump end\n");

        // Buffer assumed to be deallocated by framer
        Fw::Buffer segmentBuffer = bufferAllocate(0, (U32)segmentLen); // Port assumed to be 0
        FW_ASSERT(segmentBuffer.getSize() >= segmentLen, segmentBuffer.getSize());
        segmentBuffer.getSerializeRepr().serialize(reinterpret_cast<U8 *>(segment), segmentLen, true);
        bufferOut(0, segmentBuffer); // Port assumed to be 0

        sm_TaskYield();
    }
}

void FramerHelper::sendBundle(char *bundleBuffer, size_t size)
{
    //printf("[FramerHelper] BP: Hex dump start\n");
    //for (int i = 0; i < size; i++)
    //{
    //    printf("%02x", (unsigned char)bundleBuffer[i]);
    //}
    //printf("\n");
    //printf("[FramerHelper] BP: Hex dump end\n");

    // TODO these can be defined outside of this method instead
    // of instantiated each time this method is called.
    // Maybe make them `static`?
    Object bundleZco, bundlePayload;
    Object newBundle;   /* We never use but bp_send requires it. */

    if (pthread_mutex_lock(&(*sdrMutex)) != 0)
    {
        putErrmsg("Couldn't take sdr mutex.", NULL);
        return;
    }

    /* Wrap the linebuffer in a bundle payload. */
    oK(sdr_begin_xn(bpSdr));
    bundlePayload = sdr_malloc(bpSdr, size);
    if (bundlePayload)
    {
        sdr_write(bpSdr, bundlePayload, bundleBuffer, size);
    }

    if (sdr_end_xn(bpSdr) < 0)
    {
        pthread_mutex_unlock(&(*sdrMutex));
        bp_close(sap);
        putErrmsg("No space for bpchat payload.", NULL);
        return;
    }

    bundleZco = ionCreateZco(ZcoSdrSource, bundlePayload, 0,
        size, BP_STD_PRIORITY, 0, ZcoOutbound, NULL);
    if (bundleZco == 0 || bundleZco == (Object) ERROR)
    {
        pthread_mutex_unlock(&(*sdrMutex));
        bp_close(sap);
        putErrmsg("bpchat can't create bundle ZCO.", NULL);
        return;
    }

    pthread_mutex_unlock(&(*sdrMutex));

    /* Send the bundle payload. */
    // TODO TTL should be parameter
    if (bp_send(sap, destEid, NULL, 86400, BP_STD_PRIORITY,
            custodySwitch, 0, 0, NULL, bundleZco,
            &newBundle) <= 0)
    {
        putErrmsg("bpchat can't send bundle.", NULL);
        return;
    }
    //printf("[FramerHelper] BP: Sent %d bytes\n", size);
}

} // end namespace Dtn
