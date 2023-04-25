#ifndef Dtn_FramerHelper_HPP
#define Dtn_FramerHelper_HPP

#include "ltpP.h"

namespace Dtn
{

class FramerHelper
{
    private: // TODO accept callback function to give `Framer` bytes off of LTP

        static const uvast remoteEngineId = 3;

        void ltpFrame();

    public:

        // TODO pass remote engine ID in
        FramerHelper(uvast _remoteEngineId) {} //: remoteEngineId(_remoteEngineId)

        // Must wrap `ltpFrame()` with a static function since
        // member functions have an implicit reference to `this`
        static void *ltpFrameWrapper(void *arg);
};

} // end namespace Dtn


#endif // Dtn_FramerHelper_HPP
