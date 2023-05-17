// ======================================================================
// \title  Deframer.cpp
// \author root
// \brief  cpp file for Deframer component implementation class
// ======================================================================

#include <Dtn/Deframer/Deframer.hpp>
#include <FpConfig.hpp>

extern "C"
{
#include "bp_receive.h"
}

namespace Dtn
{

// Intentionally mutable strings instead of string literals.
// ION's `parseEidString()` does not work for string literals
static char ownEid[] = "ipn:2.1";
static char destEid[] = "ipn:3.1";

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Deframer::Deframer(const char* const compName)
: DeframerComponentBase(compName),
    dtnBuffer(Fw::Buffer(m_data, sizeof(m_data), 0)), // TODO what `context` to use?
    helper(
        DeframerHelper(
            ownEid,
            dtnBuffer,
            std::bind(&Deframer::bufferOut_out, this, std::placeholders::_1, std::placeholders::_2)))
{ }

void Deframer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    DeframerComponentBase::init(queueDepth, instance);

    pthread_t t;
    int status = pthread_create(&t, NULL, DeframerHelper::bundleReceiveWrapper, static_cast<void *>(&helper));
    if (status != 0)
    {
        printf("[Dtn.Deframer] Error creating thread\n");
    }
    pthread_detach(t);
}

Deframer::~Deframer() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Deframer::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer)
{
    // Data here is assumed to be LTP data
    DeframerHelper::ltpDeframe(fwBuffer.getData(), fwBuffer.getSize());
}

void Deframer::cmdResponseIn_handler
(
    const NATIVE_INT_TYPE portNum,
    FwOpcodeType opCode,
    U32 cmdSeq,
    const Fw::CmdResponse &response
)
{
    // TODO why is this needed in the first place? See `Deframer.fpp` comment
}

}  // end namespace Dtn
