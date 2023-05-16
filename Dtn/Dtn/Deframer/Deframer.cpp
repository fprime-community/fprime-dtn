// ======================================================================
// \title  Deframer.cpp
// \author root
// \brief  cpp file for Deframer component implementation class
// ======================================================================

#include <Dtn/Deframer/Deframer.hpp>
#include <FpConfig.hpp>
#include "DeframerHelper.hpp"

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
: DeframerComponentBase(compName)
{
}

void Deframer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    DeframerComponentBase::init(queueDepth, instance);
    printf("[Dtn.Deframer] bp_receive starting\n");
    // TODO this is where we start a thread that loops and calls `bp_receive()`
    // and passes the received data to this component's output ports.
    // The input ports here just put the data on the LTP queue
    bp_receive_init(ownEid, destEid);
    printf("[Dtn.Deframer] bp_receive started\n");

    DeframerHelper::init();
}

Deframer::~Deframer() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Deframer::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer)
{
    // Data here is assumed to be LTP data
    DeframerHelper::ltpDeframe(fwBuffer.getData(), fwBuffer.getSize());

    bufferOut_out(portNum, fwBuffer);
}

void Deframer::cmdResponseIn_handler
(
    const NATIVE_INT_TYPE portNum,
    FwOpcodeType opCode,
    U32 cmdSeq,
    const Fw::CmdResponse &response
)
{
    // TODO
}

}  // end namespace Dtn
