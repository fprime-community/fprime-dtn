#include <cstdio>
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

void DtnFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packet_type)
{
    m_internalFramingProtocol.frame(data, size, packet_type);
}

//
// DEFRAMING
//

DtnDeframing::DtnDeframing(Svc::DeframingProtocol& internalDeframingProtocol) : Svc::DeframingProtocol(),
    m_internalDeframingProtocol(internalDeframingProtocol)
{ }

Svc::DeframingProtocol::DeframingStatus DtnDeframing::deframe(Types::CircularBuffer& ring, U32& needed)
{
    return m_internalDeframingProtocol.deframe(ring, needed);
}

};
