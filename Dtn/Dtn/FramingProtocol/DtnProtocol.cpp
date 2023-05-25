#include <cstdio>
#include <pthread.h>
#include <FpConfig.hpp>
#include "DtnProtocol.hpp"

namespace Dtn
{

//
// FRAMING
//

// TODO Assumes that `_internalFramingProtocol.setup(this)` has been called.
// If `setup()` were virtual then it could be overridden to automatically setup `_internalFramingProtocol`.
// Or, if `setup()` accepted a `const Svc::FramingProtocolInterface` then `internalFramingProtocol.setup(*this)` would be possible
DtnFraming::DtnFraming
(
    char *_ownEid,
    char *_destEid,
    U64 _remoteEngineId,
    Svc::FramingProtocol& _internalFramingProtocol
) :
    Svc::FramingProtocol(),
    Svc::FramingProtocolInterface(),
    internalFramingProtocol(_internalFramingProtocol),
    helper(_ownEid, _destEid, _remoteEngineId, &m_interface)
{ }

// TODO Assumes that `setup()` is called prior to this to ensure `framer` is set
void DtnFraming::start()
{
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
    internalFramingProtocol.frame(data, size, packetType); // This calls `send()`
}

Fw::Buffer DtnFraming::allocate(const U32 size)
{
    return m_interface->allocate(size);
}

void DtnFraming::send(Fw::Buffer& outgoing)
{
    char *buffer = (char *)outgoing.getData();
    size_t n = outgoing.getSize();
    helper.sendBundle(buffer, n); // TODO have `sendBundle()` report a status
}

//
// DEFRAMING
//

DtnDeframing::DtnDeframing(Svc::DeframingProtocol& internalDeframingProtocol) : Svc::DeframingProtocol(),
    m_internalDeframingProtocol(internalDeframingProtocol)
{ }

Svc::DeframingProtocol::DeframingStatus DtnDeframing::deframe(Types::CircularBuffer& ring, U32& needed)
{
    //printf("[DtnDeframing] deframe() ring %u\t%u\n", ring.get_capacity(), ring.get_free_size());
    //ring.print();

    // TODO
    // Call m_internalDeframingProtocol.deframe()
    // - Must first `setup()` m_internalDeframingProtocol with your own DTN implementation of the ProtocolInterface
    //    - This way you can intercept any `route()` call
    // Ingest into LTP with `ltpHandleInboundSegment()`
    // Call `bp_receive()` to get LTP deframed data (blocking)
    // - How many times should this be called though? One LTP segment may contain multiple bundles
    //    - This issue also applies to GDS DTN fork
    //    - Could enter while loop and call `bp_receive()` with `BP_POLL` argument 
    //        - Once a BP is received keep polling until no bundle is received
    //        - This wouldn't work if large files are being transferred
    // Call route() with this data
    return m_internalDeframingProtocol.deframe(ring, needed);
}

};
