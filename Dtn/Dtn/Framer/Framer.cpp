// ======================================================================
// \title  Framer.cpp
// \author rosemurg
// \brief  cpp file for Framer component implementation class
// ======================================================================

#include <cstring>
#include <pthread.h>
#include <Fw/Com/ComPacket.hpp>
#include <Fw/Types/Serializable.hpp>
#include <Dtn/Framer/Framer.hpp>
#include <FpConfig.hpp>

namespace Dtn
{

// Intentionally mutable strings instead of string literals.
// ION's `parseEidString()` does not work for string literals
static char ownEid[]  = "ipn:2.1";
static char destEid[] = "ipn:3.1";
static const U64 remoteEngineId = 3;

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Framer::Framer(const char* const compName)
: FramerComponentBase(compName),
    helper(
        ownEid,
        destEid,
        remoteEngineId,
        std::bind(&Framer::bufferAllocate_out, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Framer::bufferOut_out, this, std::placeholders::_1, std::placeholders::_2))
{ }

void Framer::configure(pthread_mutex_t& sdrMutex)
{
    helper.configure(sdrMutex);
}

void Framer::start()
{
    // TODO replace pthread with OS::Task
    pthread_t t;
    int status = pthread_create(&t, NULL, FramerHelper::ltpFrameWrapper, static_cast<void *>(&helper));
    if (status != 0)
    {
        printf("[Dtn.Framer] Error creating thread\n");
    }
    pthread_detach(t);
}

Framer::~Framer() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Framer::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer)
{
    // Prepend FW_PACKET_FILE to start of data framed file data.
    // We need this here for GDS deframing to properly detect that this data is a file,
    // com data (`Fw::ComBuffer`) comes with this packet type prepended.
    // Must allocate and deallocate a buffer since we'll need to increase size
    // to support the file packet type.

    // Allocate file buffer
    FW_ASSERT(sizeof(Fw::ComPacket::ComPacketType) == 4);
    size_t fileBufferSize = sizeof(Fw::ComPacket::ComPacketType) + fwBuffer.getSize();
    Fw::Buffer fileBuffer = bufferAllocate_out(portNum, fileBufferSize);
    FW_ASSERT(fileBuffer.getSize() >= fileBufferSize, fileBuffer.getSize());

    // Insert file packet type bytes
    Fw::SerializeStatus status;
    status = fileBuffer.getSerializeRepr().serialize(Fw::ComPacket::ComPacketType::FW_PACKET_FILE);
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

    // Copy data into file buffer
    status = fileBuffer.getSerializeRepr().serialize(
        reinterpret_cast<U8 *>(fwBuffer.getData()),
        fwBuffer.getSize(),
        true);
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);

    // Data is now copied to `fileBuffer`, free the original buffer
    bufferDeallocate_out(portNum, fwBuffer);

    helper.sendBundle((char *)fileBuffer.getData(), (size_t)fileBuffer.getSize());

    // Data is now ingested into ION, free the file buffer
    bufferDeallocate_out(portNum, fileBuffer);
}

void Framer::comIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context)
{
    // Note that `Fw::ComBuffer` size <= 128 bytes
    helper.sendBundle((char *)data.getBuffAddr(), (size_t)data.getBuffLength());
}

}  // end namespace Dtn
