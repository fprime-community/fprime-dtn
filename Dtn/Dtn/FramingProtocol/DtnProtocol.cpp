#include <FpConfig.hpp>
#include "DtnProtocol.hpp"

namespace Dtn
{

//
// FRAMING
//

DtnFraming::DtnFraming(Svc::FramingProtocol& internalFramingProtocol) : Svc::FramingProtocol(),
    m_internalFramingProtocol(internalFramingProtocol)
{ }

void DtnFraming::setup(Svc::FramingProtocolInterface& interface)
{
    Svc::FramingProtocol::setup(interface);
    m_internalFramingProtocol.setup(interface);
}

void DtnFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type)
{
    Svc::FramingProtocol::frame(data, size, packet_type);
}

//
// DEFRAMING
//

DtnDeframing::DtnDeframing(Svc::DeframingProtocol& internalDeframingProtocol) : Svc::DeframingProtocol(),
    m_internalDeframingProtocol(internalDeframingProtocol)
{ }

void DtnDeframing::setup(Svc::DeframingProtocolInterface& interface)
{
    Svc::DeframingProtocol::setup(interface);
    m_internalDeframingProtocol.setup(interface);
}

Svc::DeframingProtocol::DeframingStatus DtnDeframing::deframe(Types::CircularBuffer& ring, U32& needed)
{
    return Svc::DeframingProtocol::deframe(ring, needed);
}

};
