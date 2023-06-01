#ifndef Dtn_DtnProtocol_HPP
#define Dtn_DtnProtocol_HPP

#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocolInterface.hpp>
#include "DeframerHelper.hpp"
#include <Svc/Deframer/Deframer.hpp>

// TODO remove deframer hack
namespace Ref
{
    extern Svc::Deframer deframer;
}

namespace Dtn
{

    class DtnDeframing : public Svc::DeframingProtocol, public Svc::DeframingProtocolInterface
    {
        private:

            Svc::DeframingProtocol& internalDeframingProtocol;
            DeframerHelper helper;

        public:

            DtnDeframing
            (
                char *_ownEid,
                pthread_mutex_t& sdrMutex,
                Svc::DeframingProtocol& _internalDeframingProtocol
            );

            void start();

            //
            // DeframingProtocol
            //

            Svc::DeframingProtocol::DeframingStatus deframe
            (
                Types::CircularBuffer& buffer, //!< The circular buffer
                U32& needed                    //!< The number of bytes needed, updated by the caller
            ) override;

            //
            // DeframingProtocolInterface
            //

            Fw::Buffer allocate(const U32 size) override;
            void route(Fw::Buffer& data) override;
    };

}

#endif // Dtn_DtnProtocol_HPP
