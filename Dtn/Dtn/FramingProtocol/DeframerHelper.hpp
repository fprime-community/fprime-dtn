#ifndef Dtn_DeframerHelper_HPP
#define Dtn_DeframerHelper_HPP

#include <functional>
#include <FpConfig.hpp>
#include <Fw/Buffer/Buffer.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocolInterface.hpp>

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED

#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"

namespace Dtn
{

class DeframerHelper
{
    private:

        char *ownEid;
        Svc::DeframingProtocol& internalDeframingProtocol;
        Svc::DeframingProtocolInterface **deframer;

        BpSAP sap;

        void bundleDeframe();

    public:

        DeframerHelper
        (
            char *_ownEid,
            Svc::DeframingProtocol& _internalDeframingProtocol,
            Svc::DeframingProtocolInterface **_deframer
        );

        // Must wrap `bundleDeframe()` with a static function since
        // member functions have an implicit reference to `this`
        static void *bundleDeframeWrapper(void *self);

        // TODO have `receiveLtp()` report a status
        void receiveLtp(char *ltpBuffer, int size);
};

} // end namespace Dtn

#endif // Dtn_DeframerHelper_HPP
