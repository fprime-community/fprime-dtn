// ======================================================================
// \title  Deframer.cpp
// \author root
// \brief  cpp file for Deframer component implementation class
// ======================================================================

#include <Dtn/Deframer/Deframer.hpp>
#include <FpConfig.hpp>

namespace Dtn
{

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
