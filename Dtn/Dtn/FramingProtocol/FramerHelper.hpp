#ifndef Dtn_FramerHelper_HPP
#define Dtn_FramerHelper_HPP

#include <functional>
#include <FpConfig.hpp>
#include <Fw/Buffer/Buffer.hpp>
#include "ltpP.h"

namespace Dtn
{

typedef std::function<void(NATIVE_INT_TYPE, Fw::Buffer&)> DtnBufferOutFunc;

class FramerHelper
{
    private: // TODO accept callback function to give `Framer` bytes off of LTP

        const uvast remoteEngineId;
        Fw::Buffer& dtnBuffer;
        const DtnBufferOutFunc dtnBufferOutFunc;

        void ltpFrame();

    public:

        FramerHelper(uvast _remoteEngineId, Fw::Buffer& _dtnBuffer, DtnBufferOutFunc _dtnBufferOutFunc) :
            remoteEngineId(_remoteEngineId),
            dtnBuffer(_dtnBuffer),
            dtnBufferOutFunc(_dtnBufferOutFunc)
        { }

        // Must wrap `ltpFrame()` with a static function since
        // member functions have an implicit reference to `this`
        static void *ltpFrameWrapper(void *self);
};

} // end namespace Dtn


#endif // Dtn_FramerHelper_HPP
