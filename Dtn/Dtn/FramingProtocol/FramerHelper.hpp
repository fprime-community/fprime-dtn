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

class FramerHelper
{
    private:

        char *ownEid;
        char *destEid;
        const U64 remoteEngineId; // Same type as `uvast` (see `ltpP.h`)
        Svc::FramingProtocol& internalFramingProtocol;

        BpSAP sap;
        Sdr bpSdr;
        BpCustodySwitch custodySwitch = NoCustodyRequested;

        void ltpFrame();

    public:

        FramerHelper
        (
            char *_ownEid,
            char *_destEid,
            U64 _remoteEngineId,
            Svc::FramingProtocol& _internalFramingProtocol
        );

        // Must wrap `ltpFrame()` with a static function since
        // member functions have an implicit reference to `this`
        static void *ltpFrameWrapper(void *self);

        // TODO have `sendBundle()` report a status
        void sendBundle(char *bundleBuffer, size_t size);
};

} // end namespace Dtn

#endif // Dtn_FramerHelper_HPP
