// ======================================================================
// \title  DtnFramer.hpp
// \author root
// \brief  cpp file for DtnFramer test harness implementation class
// ======================================================================

#include "Tester.hpp"

namespace Com {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      DtnFramerGTestBase("Tester", Tester::MAX_HISTORY_SIZE),
      component("DtnFramer")
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
    toDo()
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_comBundleOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_comBundleOut(fwBuffer);
  }

  void Tester ::
    from_comStatus_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Success &condition
    )
  {
    this->pushFromPortEntry_comStatus(condition);
  }

  void Tester ::
    from_passthroughBufferOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_passthroughBufferOut(fwBuffer);
  }

  void Tester ::
    from_passthroughComOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    this->pushFromPortEntry_passthroughComOut(data, context);
  }


} // end namespace Com
