#ifndef Dtn_FramerHelper_HPP
#define Dtn_FramerHelper_HPP

#include <functional>
#include <FpConfig.hpp>
#include <Fw/Buffer/Buffer.hpp>
#include <Svc/FramingProtocol/FramingProtocolInterface.hpp>

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED

#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"

namespace Dtn
{

class FramerHelper
{
    private:

        // TODO or just call framer->allocate() to create a buffer on-the-fly?
        U8 m_data[1024]; // 1 KiB
        Fw::Buffer dtnBuffer = Fw::Buffer(m_data, sizeof(m_data), 200); // TODO what `context` to use?

        char *ownEid;
        char *destEid;
        const U64 remoteEngineId; // Same type as `uvast` (see `ltpP.h`)
        Svc::FramingProtocolInterface **framer;

        BpSAP sap;
        Sdr bpSdr;
        BpCustodySwitch custodySwitch = NoCustodyRequested;
        pthread_mutex_t sdrmutex = PTHREAD_MUTEX_INITIALIZER;

        void ltpFrame();

    public:

        FramerHelper
        (
            char *_ownEid,
            char *_destEid,
            U64 _remoteEngineId,
            Svc::FramingProtocolInterface **framer
        );

        // Must wrap `ltpFrame()` with a static function since
        // member functions have an implicit reference to `this`
        static void *ltpFrameWrapper(void *self);

        void sendBundle(char *bundleBuffer, size_t size);
};

} // end namespace Dtn


#endif // Dtn_FramerHelper_HPP
