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
    // TODO
  }

  void DtnFramer ::
    comIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    // TODO
  }

  void DtnFramer ::
    comStatusIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Success &condition
    )
  {
    // TODO
  }

} // end namespace Com
