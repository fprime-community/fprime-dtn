//!
//! @file TODO
//! @brief Reads lines from stdin and sends these lines in bundles.
//!     Receives bundles and writes them to stdout
//!

#include <cstdio>
// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED
#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"
#include "ltpP.h"
#include "DeframerHelper.hpp"

namespace Dtn
{

void *DeframerHelper::bundleReceiveWrapper(void *self)
{
    DeframerHelper& instance = *static_cast<DeframerHelper *>(self);
    instance.bundleReceive();
    return nullptr;
}

void DeframerHelper::bundleReceive()
{
    BpSAP sap;
    pthread_mutex_t sdrmutex = PTHREAD_MUTEX_INITIALIZER;

    printf("[Dtn.DeframerHelper] Receive starting\n");
    if (ltpInit(0) < 0)
    {
        printf("[Dtn.DeframerHelper] Error initializing LTP\n");
    }

    if (bp_attach() < 0)
    {
        putErrmsg("Can't bp_attach()", NULL);
        exit(1);
    }

    if (bp_open(ownEid, &sap) < 0)
    {
        putErrmsg("Can't open own endpoint.", ownEid);
        exit(1);
    }
    printf("[Dtn.DeframerHelper] Receive started\n");

    Sdr sdr = bp_get_sdr();

    BpDelivery dlv;
    ZcoReader reader;
    int bundleLenRemaining;
    int rc;
    int bytesToRead;
    char *bundleBuffer = (char *)dtnBuffer.getData();

    int running = 1;
    while (running)
    {
        if (bp_receive(sap, &dlv, BP_BLOCKING) < 0)
        {
            putErrmsg("bpchat bundle reception failed.", NULL);
            break;
        }

        if (dlv.result == BpReceptionInterrupted || dlv.adu == 0)
        {
            bp_release_delivery(&dlv, 1);
            continue;
        }

        if (dlv.result == BpEndpointStopped)
        {
            bp_release_delivery(&dlv, 1);
            break;
        }

        if (pthread_mutex_lock(&sdrmutex) != 0)
        {
            putErrmsg("Couldn't take sdr mutex.", NULL);
            break;
        }

        oK(sdr_begin_xn(sdr));
        int bundleSize = zco_source_data_length(sdr, dlv.adu);
        zco_start_receiving(dlv.adu, &reader);
        int bundleLenRemaining = bundleSize;
        while (bundleLenRemaining > 0)
        {
            bytesToRead = MIN(bundleLenRemaining, sizeof(bundleBuffer)-1);
            rc = zco_receive_source(sdr, &reader, bytesToRead, bundleBuffer);
            if (rc < 0)
            {
                break;
            }
            bundleLenRemaining -= rc;
        }
        bundleSize -= bundleLenRemaining;

        dtnBuffer.setSize(bundleSize);
        bufferOutFunc(0, dtnBuffer);

        if (sdr_end_xn(sdr) < 0)
        {
            running = 0;
        }

        pthread_mutex_unlock(&sdrmutex);
        bp_release_delivery(&dlv, 1);
    }
}

// Don't need to have ownership over buffer since it's copied to ION by `ltpHandleInboundSegment()`
void DeframerHelper::ltpDeframe(unsigned char *buffer, int segmentSize)
{
    // TODO the ION API accepts only a `char` here, not an `unsigned char`.
    // This should be O.K. as long as it's assumed that the target platform treats `char` as `unsigned char`
    if (ltpHandleInboundSegment((char *)buffer, segmentSize) < 0)
    {
        printf("[Dtn.DeframerHelper] Unable to ingest inbound LTP segment\n");
    }
    printf("[Dtn.DeframerHelper] Enqueued %d bytes\n", segmentSize);
}

} // end namespace Dtn
