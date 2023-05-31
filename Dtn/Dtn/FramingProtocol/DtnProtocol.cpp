#include <cstdio>
#include <pthread.h>
#include <FpConfig.hpp>
#include "DtnProtocol.hpp"

namespace Dtn
{

DtnDeframing::DtnDeframing
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
    size_t segmentLen = (size_t)data.getSize();
    char *segment = (char *)data.getData();

    //printf("[DeframerHelper] LTP: Hex dump start\n");
    //for (int i = 0; i < segmentLen; i++)
    //{
    //    printf("%02x", (unsigned char)segment[i]);
    //}
    //printf("\n");
    //printf("[DeframerHelper] LTP: Hex dump end\n");

    helper.receiveLtp(segment, (int)segmentLen);
}

};
