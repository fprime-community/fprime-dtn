//!
//! @file TODO
//! @brief Reads lines from stdin and sends these lines in bundles.
//!     Receives bundles and writes them to stdout
//!

#include <cstdio>
#include "ltpP.h"
#include "FramerHelper.hpp"

namespace Dtn
{

void *FramerHelper::ltpFrameWrapper(void *arg)
{
    // TODO static_cast cannot be used here since it doesn't preserve member variables.
    // For now `remoteEngineId` will be a static member to work around this
    FramerHelper *instance = static_cast<FramerHelper *>(arg);
    instance->ltpFrame();
    return nullptr;
}

void FramerHelper::ltpFrame()
{
    if (ltpInit(0) < 0)
    {
        printf("[Dtn.FramerHelper] Error initializing LTP\n");
    }

    LtpVspan *vspan;
    PsmAddress vspanElt; // Is an unsigned long

    Sdr sdr = getIonsdr();
    if (!sdr_begin_xn(sdr)) // Needed to get "lock" (see `udplso.c`)
    {
        printf("[Dtn.FramerHelper] Error starting SDR transaction\n");
        return; // TODO clean up this return logic
    }

    findSpan(remoteEngineId, &vspan, &vspanElt);
    if (vspanElt == 0)
    {
        sdr_exit_xn(sdr);
        printf("[Dtn.FramerHelper] No such engine in database: %llu\n", remoteEngineId);
        return; // TODO clean up this return logic
    }

    sdr_exit_xn(sdr);
    printf("[Dtn.FramerHelper] Found span: %lu\n", vspanElt);

    char *segment;
    for (;;)
    {
        int segmentLen = ltpDequeueOutboundSegment(vspan, &segment);
        if (segmentLen < 0)
        {
            printf("[Dtn.FramerHelper] Bad segment length: %d\n", segmentLen);
            return;
        }
        printf("[Dtn.FramerHelper] Dequeued %d bytes\n", segmentLen);

        printf("[Dtn.FramerHelper] Hex dump start\n");
        for (int i = 0; i < segmentLen; i++)
        {
            printf("%02x ", (unsigned char)segment[i]);
        }
        printf("\n");
        printf("[Dtn.FramerHelper] Hex dump end\n");

        sm_TaskYield();
    }
}

} // end namespace Dtn
