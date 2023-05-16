#ifndef Dtn_DeframerHelper_HPP
#define Dtn_DeframerHelper_HPP

#include <functional>
#include <FpConfig.hpp>
#include <Fw/Buffer/Buffer.hpp>

namespace Dtn
{

typedef std::function<void(NATIVE_INT_TYPE, Fw::Buffer&)> BufferOutFunc;

class DeframerHelper
{
    private:

        char *ownEid;
        Fw::Buffer& dtnBuffer;
        const BufferOutFunc bufferOutFunc;

        void bundleReceive();

    public:

        DeframerHelper(char *_ownEid, Fw::Buffer& _dtnBuffer, BufferOutFunc _bufferOutFunc) :
            ownEid(_ownEid),
            dtnBuffer(_dtnBuffer),
            bufferOutFunc(_bufferOutFunc)
        { }

        // Must wrap `bundleReceive()` with a static function since
        // member functions have an implicit reference to `this`
        static void *bundleReceiveWrapper(void *self);

        static void ltpDeframe(unsigned char *buffer, int segmentSize);
};

} // end namespace Dtn


#endif // Dtn_DeframerHelper_HPP
