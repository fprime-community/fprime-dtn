// ======================================================================
// \title  DtnFramer.cpp
// \author rosemurg
// \brief  cpp file for DtnFramer component implementation class
// ======================================================================


#include <DtnRef/Com/Dtn/DtnFramer/DtnFramer.hpp>
#include <FpConfig.hpp>

extern "C" {
#include "bpchat.h"
}

namespace Com {

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
    printf("[DTN] bpchat starting\n");
    // Intentionally mutable strings instead of string literals.
    // ION's `parseEidString()` does not work for string literals
    char ownEid[] = "ipn:2.1";
    char destEid[] = "ipn:3.1";
    bpchat_start(ownEid, destEid);
    printf("[DTN] bpchat started\n");
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

    // TODO
    // printf("[DtnFramer] data (%zu) = %x\n",
    //     data.getBuffCapacity(),
    //     data.getBuffAddr());

    // char msg[] = "hello";
    // //if (!bpchat_send((char *)data.getBuffAddr(), data.getBuffCapacity()))
    // if (!bpchat_send(msg, 5))
    // {
    //   printf("[DtnFramer] bpchat_send failed\n");
    // }
    // //this->comBundleOut_out(0, bpBuffer);
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
