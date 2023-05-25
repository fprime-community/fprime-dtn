#ifndef Dtn_DtnProtocol_HPP
#define Dtn_DtnProtocol_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>
#include "FramerHelper.hpp"

namespace Dtn
{

    class DtnFraming : public Svc::FramingProtocol, public Svc::FramingProtocolInterface
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

            //
            // FramingProtocolInterface
            //

            Fw::Buffer allocate(const U32 size) override;
            void send(Fw::Buffer& outgoing) override;

    };

    class DtnDeframing : public Svc::DeframingProtocol // TODO , Svc::DeframingProtocolInterface
    {
        private:

            Svc::DeframingProtocol& m_internalDeframingProtocol;

        public:

            DtnDeframing(Svc::DeframingProtocol& internalDeframingProtocol);

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
    };

}

#endif // Dtn_DtnProtocol_HPP
