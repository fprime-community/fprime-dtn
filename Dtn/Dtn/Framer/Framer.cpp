// ======================================================================
// \title  Framer.cpp
// \author rosemurg
// \brief  cpp file for Framer component implementation class
// ======================================================================


#include <cstring>
#include <Dtn/Framer/Framer.hpp>
#include <FpConfig.hpp>

extern "C" {
#include "bpchat.h"
}

namespace Dtn {

// Intentionally mutable strings instead of string literals.
// ION's `parseEidString()` does not work for string literals
static char ownEid[]  = "ipn:2.1";
static char destEid[] = "ipn:3.1";

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  Framer ::
    Framer(
        const char *const compName
    ) : FramerComponentBase(compName)
  {

  }

  void Framer ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
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
  }

  Framer ::
    ~Framer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Framer ::
    bufferIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->passthroughBufferOut_out(0, fwBuffer);
    //this->comBundleOut_out(0, bpBuffer);
  }

  void Framer ::
    comIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    this->passthroughComOut_out(0, data, context);

    //printf("[Dtn.Framer] data (%zu) = %x\n",
    //    data.getBuffCapacity(),
    //    data.getBuffAddr());

    char buffer[128]; // data.getBuffCapacity() == 128
    memcpy(buffer, data.getBuffAddr(), data.getBuffCapacity());
    buffer[0] = '\xC0';
    buffer[1] = '\xDE';
    buffer[126] = '\xC0';
    buffer[127] = '\xDA';

    if (!bpchat_send(buffer, 128))
    {
      printf("[Dtn.Framer] bpchat_send failed\n");
    }

    //char msg[] = "paul\n";
    //if (!bpchat_send(msg, 5))
    //{
    //  printf("[Dtn.Framer] bpchat_send failed\n");
    //}

    //this->comBundleOut_out(0, bpBuffer);
  }

  void Framer ::
    comStatusIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Success &condition
    )
  {
    printf("[Dtn.Framer] comStatusIn_handler\n");
  }

} // end namespace Dtn
