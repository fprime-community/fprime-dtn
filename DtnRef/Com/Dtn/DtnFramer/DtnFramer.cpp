// ======================================================================
// \title  DtnFramer.cpp
// \author rosemurg
// \brief  cpp file for DtnFramer component implementation class
// ======================================================================


#include <cstring>
#include <DtnRef/Com/Dtn/DtnFramer/DtnFramer.hpp>
#include <FpConfig.hpp>

extern "C" {
#include "bpchat.h"
}

namespace Com {

// Intentionally mutable strings instead of string literals.
// ION's `parseEidString()` does not work for string literals
static char ownEid[]  = "ipn:2.1";
static char destEid[] = "ipn:3.1";

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  DtnFramer ::
    DtnFramer(
        const char *const compName
    ) : DtnFramerComponentBase(compName)
  {

  }

  void DtnFramer ::
    init(
        const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    DtnFramerComponentBase::init(queueDepth, instance);

    printf("[DtnFramer] bpchat starting\n");
    bpchat_start(ownEid, destEid);
    printf("[DtnFramer] bpchat started\n");

    char m1[] = "first\n";
    if (!bpchat_send(m1, 6))
    {
      printf("[DtnFramer] bpchat_send failed\n");
    }
  }

  DtnFramer ::
    ~DtnFramer()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void DtnFramer ::
    bufferIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->passthroughBufferOut_out(0, fwBuffer);
    //this->comBundleOut_out(0, bpBuffer);
  }

  void DtnFramer ::
    comIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    this->passthroughComOut_out(0, data, context);

    //printf("[DtnFramer] data (%zu) = %x\n",
    //    data.getBuffCapacity(),
    //    data.getBuffAddr());

    char buffer[128];
    memcpy(buffer, data.getBuffAddr(), data.getBuffCapacity());
    buffer[0] = 'h';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = 'o';

    if (!bpchat_send(buffer, data.getBuffCapacity()))
    {
      printf("[DtnFramer] bpchat_send failed\n");
    }

    //char msg[] = "paul\n";
    //if (!bpchat_send(msg, 5))
    //{
    //  printf("[DtnFramer] bpchat_send failed\n");
    //}

    //this->comBundleOut_out(0, bpBuffer);
  }

  void DtnFramer ::
    comStatusIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Success &condition
    )
  {
    printf("[DtnFramer] comStatusIn_handler\n");
  }

} // end namespace Com
