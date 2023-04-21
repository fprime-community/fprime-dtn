//!
//! @file TODO
//! @brief Reads lines from stdin and sends these lines in bundles.
//!     Receives bundles and writes them to stdout
//!

#include <cstdio>
#include "FramerHelper.hpp"

namespace Dtn
{

void *FramerHelper::paul(void *args)
{
    // TODO call `ltpDequeueOutboundSegment()`

    for (;;)
    {
        printf("HIT PAUL\n");
    }
    return nullptr;
}

} // end namespace Dtn
