#ifndef Dtn_DtnProtocol_HPP
#define Dtn_DtnProtocol_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocolInterface.hpp>
#include "FramerHelper.hpp"
#include "DeframerHelper.hpp"

namespace Dtn
{

    class DtnFraming : public Svc::FramingProtocol
    {
        private:

            Svc::FramingProtocol& internalFramingProtocol;
            FramerHelper helper;

        public:

            DtnFraming
            (
                char *_ownEid,
                char *_destEid,
                U64 _remoteEngineId,
                Svc::FramingProtocol& _internalFramingProtocol
            );

            void start();

            //
            // FramingProtocol
            //

            void frame
            (
                const U8* const data,                   //!< The data
                const U32 size,                         //!< The data size in bytes
                Fw::ComPacket::ComPacketType packetType //!< The packet type
            ) override;
    };

    class DtnDeframing : public Svc::DeframingProtocol, public Svc::DeframingProtocolInterface
    {
        private:

            Svc::DeframingProtocol& internalDeframingProtocol;
            DeframerHelper helper;

        public:

            DtnDeframing(char *_ownEid, Svc::DeframingProtocol& _internalDeframingProtocol);

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
