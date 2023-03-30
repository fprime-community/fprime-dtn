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
: DeframerComponentBase(compName)
{
}

void Deframer::init(const NATIVE_INT_TYPE queueDepth, const NATIVE_INT_TYPE instance)
{
    DeframerComponentBase::init(queueDepth, instance);
    printf("[Dtn.Deframer] bp_receive starting\n");
    bp_receive_init(ownEid, destEid);
    printf("[Dtn.Deframer] bp_receive started\n");
}

Deframer::~Deframer() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Deframer::bufferIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& fwBuffer)
{
    // TODO
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

void Deframer::passthroughComIn_handler(const NATIVE_INT_TYPE portNum, Fw::ComBuffer &data, U32 context)
{
    // TODO
}

}  // end namespace Dtn
