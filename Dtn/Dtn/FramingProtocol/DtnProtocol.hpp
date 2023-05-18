#ifndef Dtn_DtnProtocol_HPP
#define Dtn_DtnProtocol_HPP

#include <Svc/FramingProtocol/FramingProtocol.hpp>
#include <Svc/FramingProtocol/DeframingProtocol.hpp>

namespace Dtn
{

    class DtnFraming : public Svc::FramingProtocol
    {
        private:

            Svc::FramingProtocol& m_internalFramingProtocol;

        public:

            DtnFraming(Svc::FramingProtocol& internalFramingProtocol);

            void frame
            (
                const U8* const data,                    //!< The data
                const U32 size,                          //!< The data size in bytes
                Fw::ComPacket::ComPacketType packet_type //!< The packet type
            ) override;
    };

    class DtnDeframing : public Svc::DeframingProtocol
    {
        private:

            Svc::DeframingProtocol& m_internalDeframingProtocol;

        public:

            DtnDeframing(Svc::DeframingProtocol& internalDeframingProtocol);

            Svc::DeframingProtocol::DeframingStatus deframe
            (
                Types::CircularBuffer& buffer, //!< The circular buffer
                U32& needed                    //!< The number of bytes needed, updated by the caller
            ) override;
    };

}

#endif // Dtn_DtnProtocol_HPP
