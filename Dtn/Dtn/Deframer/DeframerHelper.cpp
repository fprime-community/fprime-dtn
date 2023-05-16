//!
//! @file TODO
//! @brief Reads lines from stdin and sends these lines in bundles.
//!     Receives bundles and writes them to stdout
//!

#include <cstdio>
#include "ltpP.h"
#include "DeframerHelper.hpp"

namespace Dtn
{

void DeframerHelper::init() // TODO remove this function, just call `ltpInit()` before loop within helper
{
    if (ltpInit(0) < 0)
    {
        printf("[Dtn.FramerHelper] Error initializing LTP\n");
    }
}

// TODO pass in FwBuffer or whatever here
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
