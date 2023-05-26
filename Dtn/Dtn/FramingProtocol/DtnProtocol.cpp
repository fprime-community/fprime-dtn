#include <cstdio>
#include <pthread.h>
#include <FpConfig.hpp>
#include "DtnProtocol.hpp"

namespace Dtn
{

//
// FRAMING
//

DtnFraming::DtnFraming
(
    char *_ownEid,
    char *_destEid,
    U64 _remoteEngineId,
    Svc::FramingProtocol& _internalFramingProtocol
) :
    Svc::FramingProtocol(),
    internalFramingProtocol(_internalFramingProtocol),
    helper(_ownEid, _destEid, _remoteEngineId, _internalFramingProtocol)
{ }

void DtnFraming::start()
{
    // TODO replace pthread with OS::Task
    pthread_t t;
    int status = pthread_create(&t, NULL, FramerHelper::ltpFrameWrapper, static_cast<void *>(&helper));
    if (status != 0)
    {
        printf("[DtnFraming] Error creating thread\n");
    }
    pthread_detach(t);
}

void DtnFraming::frame(const U8* const data, const U32 size, Fw::ComPacket::ComPacketType packetType)
{
    helper.sendBundle((char *)data, (size_t)size);
}

//
// DEFRAMING
//

DtnDeframing::DtnDeframing
(
    char *_ownEid,
    Svc::DeframingProtocol& _internalDeframingProtocol
) :
    Svc::DeframingProtocol(),
    Svc::DeframingProtocolInterface(),
    internalDeframingProtocol(_internalDeframingProtocol),
    helper(_ownEid, _internalDeframingProtocol, &m_interface)
{ }

void DtnDeframing::start()
{
    // TODO replace pthread with OS::Task
    pthread_t t;
    int status = pthread_create(&t, NULL, DeframerHelper::bundleDeframeWrapper, static_cast<void *>(&helper));
    if (status != 0)
    {
        printf("[DtnDeframing] Error creating thread\n");
    }
    pthread_detach(t);
}

Svc::DeframingProtocol::DeframingStatus DtnDeframing::deframe(Types::CircularBuffer& ring, U32& needed)
{
    return internalDeframingProtocol.deframe(ring, needed); // This calls `route()`
}

Fw::Buffer DtnDeframing::allocate(const U32 size)
{
    return m_interface->allocate(size);
}

void DtnDeframing::route(Fw::Buffer& data)
{
    printf("[DeframerHelper] LTP: Hex dump start\n");
    size_t segmentLen = (size_t)data.getSize();
    char *segment = (char *)data.getData();
    for (int i = 0; i < segmentLen; i++)
    {
        printf("%02x ", (unsigned char)segment[i]);
    }
    printf("\n");
    printf("[DeframerHelper] LTP: Hex dump end\n");

    helper.receiveLtp(segment, (int)segmentLen);
}

};
