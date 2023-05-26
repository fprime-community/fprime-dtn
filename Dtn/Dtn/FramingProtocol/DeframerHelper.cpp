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
#include "ThreadHelper.hpp" // For `sdrMutex`
#include "DeframerHelper.hpp"

namespace Dtn
{

DeframerHelper::DeframerHelper
(
    char *_ownEid,
    Svc::DeframingProtocol& _internalDeframingProtocol,
    Svc::DeframingProtocolInterface **_deframer
) :
    ownEid(_ownEid),
    internalDeframingProtocol(_internalDeframingProtocol),
    deframer(_deframer)
{
    if (bp_attach() < 0)
    {
        printf("[DeframerHelper] Can't bp_attach()\n");
    }

    if (bp_open(ownEid, &sap) < 0)
    {
        putErrmsg("Can't open own endpoint.", ownEid);
        exit(1);
    }
} 

void *DeframerHelper::bundleDeframeWrapper(void *self)
{
    DeframerHelper& instance = *static_cast<DeframerHelper *>(self);
    instance.bundleDeframe();
    return nullptr;
}

void DeframerHelper::bundleDeframe()
{
    printf("[DeframerHelper] Receive starting\n");
    if (ltpInit(0) < 0)
    {
        printf("[DeframerHelper] Error initializing LTP\n");
    }

    if (pthread_mutex_lock(&sdrMutex) != 0)
    {
        putErrmsg("Couldn't take sdr mutex.", NULL);
        return;
    }

    Sdr sdr = bp_get_sdr();

    pthread_mutex_unlock(&sdrMutex);

    BpDelivery dlv;
    ZcoReader reader;
    int bundleLenRemaining;
    int rc;
    int bytesToRead;
    Fw::Buffer bundleBuffer = (*deframer)->allocate(1024);
    // TODO FW_ASSERT(bundleBuffer.getSize() >= 1024, bundleBuffer.getSize());
    char *buffer = (char *)bundleBuffer.getData();

    for (int running = 1; running;)
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

        if (pthread_mutex_lock(&sdrMutex) != 0)
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
            bytesToRead = MIN(bundleLenRemaining, sizeof(buffer)-1);
            rc = zco_receive_source(sdr, &reader, bytesToRead, buffer);
            if (rc < 0)
            {
                break;
            }
            bundleLenRemaining -= rc;
        }
        bundleSize -= bundleLenRemaining;

        bundleBuffer.setSize(bundleSize);
        (*deframer)->route(bundleBuffer);

        if (sdr_end_xn(sdr) < 0)
        {
            running = 0;
        }

        pthread_mutex_unlock(&sdrMutex);
        bp_release_delivery(&dlv, 1);
    }
}

// Don't need to have ownership over buffer since it's copied to ION by `ltpHandleInboundSegment()`
void DeframerHelper::receiveLtp(char *ltpBuffer, int size)
{
    // TODO the ION API accepts only a `char` here, not an `unsigned char`.
    // This should be O.K. as long as it's assumed that the target platform treats `char` as `unsigned char`
    if (ltpHandleInboundSegment(ltpBuffer, size) <= 0)
    {
        printf("[DeframerHelper] Unable to ingest inbound LTP segment\n");
    }
    printf("[DeframerHelper] Enqueued %d bytes\n", size);
}

} // end namespace Dtn
