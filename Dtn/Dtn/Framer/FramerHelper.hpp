#ifndef Dtn_FramerHelper_HPP
#define Dtn_FramerHelper_HPP

#include <functional>
#include <FpConfig.hpp>
#include <Fw/Buffer/Buffer.hpp>
#include <Svc/FramingProtocol/FramingProtocol.hpp>

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED

#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"

namespace Dtn
{

typedef std::function<Fw::Buffer(NATIVE_INT_TYPE, U32)> BufferAllocateFunc;
typedef std::function<void(NATIVE_INT_TYPE, Fw::Buffer&)> BufferOutFunc;

class FramerHelper
{
    private:

        char *ownEid;
        char *destEid;
        const U64 remoteEngineId; // Same type as `uvast` (see `ltpP.h`)
        const BufferAllocateFunc bufferAllocate;
        const BufferOutFunc bufferOut;

        BpSAP sap;
        BpCustodySwitch custodySwitch = NoCustodyRequested;

        // Late-initialized by configure step
        Sdr bpSdr;
        pthread_mutex_t *sdrMutex;

        void ltpFrame();

    public:

        FramerHelper
        (
            char *_ownEid,
            char *_destEid,
            U64 _remoteEngineId,
            BufferAllocateFunc _bufferAllocate,
            BufferOutFunc _bufferOut
        );

        void configure(pthread_mutex_t& _sdrMutex);

        // Must wrap `ltpFrame()` with a static function since
        // member functions have an implicit reference to `this`
        static void *ltpFrameWrapper(void *self);

        // TODO have `sendBundle()` report a status
        void sendBundle(char *bundleBuffer, size_t size);
};

} // end namespace Dtn

#endif // Dtn_FramerHelper_HPP
