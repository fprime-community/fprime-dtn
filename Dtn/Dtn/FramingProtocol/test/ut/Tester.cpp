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
    from_dtnBufferOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_dtnBufferOut(fwBuffer);
  }

  void Tester ::
    from_comStatus_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Success &condition
    )
  {
    this->pushFromPortEntry_comStatus(condition);
  }

} // end namespace Dtn
