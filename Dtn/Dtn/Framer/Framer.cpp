// ======================================================================
// \title  Framer.cpp
// \author rosemurg
// \brief  cpp file for Framer component implementation class
// ======================================================================

#include <cstring>
#include <pthread.h>
#include <Dtn/Framer/Framer.hpp>
#include <FpConfig.hpp>

extern "C"
{
#include "bpchat.h"
}

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
    dtnBuffer(Fw::Buffer(m_data, sizeof(m_data), 0)), // TODO what `context` to use?
    helper(
        FramerHelper(
            remoteEngineId,
            dtnBuffer,
            std::bind(&Framer::dtnBufferOut_out, this, std::placeholders::_1, std::placeholders::_2)))
{ }

void Framer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    FramerComponentBase::init(queueDepth, instance);

    // TODO integrate `bpchat.c` into `FramerHelper`
    printf("[Dtn.Framer] bpchat starting\n");
    bpchat_start(ownEid, destEid);
    printf("[Dtn.Framer] bpchat started\n");

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
    char *buffer = (char *)fwBuffer.getData();
    size_t n = fwBuffer.getSize();
    if (!bpchat_send(buffer, n))
    {
        printf("[Dtn.Framer] bpchat_send failed\n");
    }
}

void Framer::comIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context)
{
    //char buffer[128];  // data.getBuffCapacity() == 128
    //memcpy(buffer, data.getBuffAddr(), data.getBuffCapacity());
    //buffer[0] = '\xC0';
    //buffer[1] = '\xDE';
    //buffer[126] = '\xC0';
    //buffer[127] = '\xDA';

    char *buffer = (char *)data.getBuffAddr();
    size_t n = data.getBuffCapacity(); // `Fw::ComBuffer` is 128 bytes
    if (!bpchat_send(buffer, n))
    {
        printf("[Dtn.Framer] bpchat_send failed\n");
    }
}

void Framer::comStatusIn_handler(const NATIVE_INT_TYPE portNum, Fw::Success& condition)
{
    printf("[Dtn.Framer] comStatusIn_handler\n");
    comStatus_out(portNum, condition);
}

}  // end namespace Dtn
