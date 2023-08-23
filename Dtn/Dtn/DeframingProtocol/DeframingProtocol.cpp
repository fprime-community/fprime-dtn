#include <cstdio>
#include <pthread.h>
#include <FpConfig.hpp>
#include "DeframingProtocol.hpp"

namespace Dtn
{

DeframingProtocol::DeframingProtocol
(
    char *_ownEid,
    pthread_mutex_t& sdrMutex,
    Svc::DeframingProtocol& _internalDeframingProtocol
) :
    Svc::DeframingProtocol(),
    Svc::DeframingProtocolInterface(),
    internalDeframingProtocol(_internalDeframingProtocol),
    helper(_ownEid, sdrMutex, _internalDeframingProtocol, &m_interface)
{ }

void DeframingProtocol::start()
{
    // TODO replace pthread with OS::Task
    pthread_t t;
    int status = pthread_create(&t, NULL, DeframerHelper::bundleDeframeWrapper, static_cast<void *>(&helper));
    if (status != 0)
    {
        printf("[DeframingProtocol] Error creating thread\n");
    }
    pthread_detach(t);
}

Svc::DeframingProtocol::DeframingStatus DeframingProtocol::deframe(Types::CircularBuffer& ring, U32& needed)
{
    return internalDeframingProtocol.deframe(ring, needed); // This calls `route()`
}

Fw::Buffer DeframingProtocol::allocate(const U32 size)
{
    return m_interface->allocate(size);
}

void DeframingProtocol::route(Fw::Buffer& data)
{
    size_t segmentLen = (size_t)data.getSize();
    char *segment = (char *)data.getData();

    printf("[DeframingProtocol] LTP: Hex dump start\n");
    for (int i = 0; i < segmentLen; i++)
    {
        printf("%02x", (unsigned char)segment[i]);
    }
    printf("\n");
    printf("[DeframingProtocol] LTP: Hex dump end\n");

    helper.receiveLtp(segment, (int)segmentLen);
    Ref::deframer.bufferDeallocate_out(0, data);
}

};
