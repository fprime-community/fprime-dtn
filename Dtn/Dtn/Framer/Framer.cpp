// ======================================================================
// \title  Framer.cpp
// \author rosemurg
// \brief  cpp file for Framer component implementation class
// ======================================================================

#include <cstring>
#include <pthread.h>
#include <Dtn/Framer/Framer.hpp>
#include <FpConfig.hpp>
#include "FramerHelper.hpp"

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
: FramerComponentBase(compName)
{
}

void Framer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    FramerComponentBase::init(queueDepth, instance);

    printf("[Dtn.Framer] bpchat starting\n");
    bpchat_start(ownEid, destEid);
    printf("[Dtn.Framer] bpchat started\n");

    char m1[] = "first\n";
    if (!bpchat_send(m1, 6))
    {
        printf("[Dtn.Framer] bpchat_send failed\n");
    }

    FramerHelper helper(remoteEngineId);

    // TODO start a thread here that loops with `ltpDequeueOutboundSegment()` (done)
    // and sends this data with: `this->dtnBufferOut_out(0, bpBuffer);`       (not done)
    // TODO where should `bpBuffer` be initialized?
    pthread_t t;
    int status = pthread_create(&t, NULL, FramerHelper::ltpFrameWrapper, &helper);
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
    // this->dtnBufferOut_out(0, bpBuffer);
}

void Framer::comIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context)
{
    // printf("[Dtn.Framer] data (%zu) = %x\n",
    //     data.getBuffCapacity(),
    //     data.getBuffAddr());

    char buffer[128];  // data.getBuffCapacity() == 128
    memcpy(buffer, data.getBuffAddr(), data.getBuffCapacity());
    buffer[0] = '\xC0';
    buffer[1] = '\xDE';
    buffer[126] = '\xC0';
    buffer[127] = '\xDA';

    if (!bpchat_send(buffer, 128))
    {
        printf("[Dtn.Framer] bpchat_send failed\n");
    }

    // this->dtnBufferOut_out(0, bpBuffer);
}

void Framer::comStatusIn_handler(const NATIVE_INT_TYPE portNum, Fw::Success& condition)
{
    printf("[Dtn.Framer] comStatusIn_handler\n");
    comStatus_out(portNum, condition);
}

}  // end namespace Dtn
