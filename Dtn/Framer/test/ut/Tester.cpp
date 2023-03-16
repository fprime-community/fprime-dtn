// ======================================================================
// \title  Framer.hpp
// \author root
// \brief  cpp file for Framer test harness implementation class
// ======================================================================

#include "Tester.hpp"

namespace Dtn {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      FramerGTestBase("Tester", Tester::MAX_HISTORY_SIZE),
      component("Framer")
  {
    // TODO implement these
    //this->initComponents();
    //this->connectPorts();
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


} // end namespace Dtn
