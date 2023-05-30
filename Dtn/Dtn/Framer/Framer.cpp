// ======================================================================
// \title  Framer.cpp
// \author rosemurg
// \brief  cpp file for Framer component implementation class
// ======================================================================

#include <cstring>
#include <pthread.h>
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

void Framer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    FramerComponentBase::init(queueDepth, instance);
}

void Framer::configure(pthread_mutex_t& sdrMutex)
{
    helper.configure(sdrMutex);
    printf("[Dtn.Framer] end of `configure` HIT\n");
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
    printf("[Dtn.Framer] end of `start` HIT\n");
}

Framer::~Framer() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Framer::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer)
{
    printf("[Dtn.Framer] bufferIn_handler HIT\n");
    helper.sendBundle((char *)fwBuffer.getData(), (size_t)fwBuffer.getSize());

    // Data is now ingested into ION, free the original buffer
    bufferDeallocate_out(portNum, fwBuffer);
}

void Framer::comIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer& data, U32 context)
{
    printf("[Dtn.Framer] comIn_handler HIT\n"); // TODO never hit
    // Note that `Fw::ComBuffer` is 128 bytes
    helper.sendBundle((char *)data.getBuffAddr(), (size_t)data.getBuffCapacity());
}

}  // end namespace Dtn
