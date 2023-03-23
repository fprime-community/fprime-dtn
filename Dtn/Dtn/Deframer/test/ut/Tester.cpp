// ======================================================================
// \title  Deframer.hpp
// \author root
// \brief  cpp file for Deframer test harness implementation class
// ======================================================================

#include "Tester.hpp"

namespace Dtn {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      DeframerGTestBase("Tester", Tester::MAX_HISTORY_SIZE),
      component("Deframer")
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
    from_bufferOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    this->pushFromPortEntry_bufferOut(fwBuffer);
  }

  void Tester ::
    from_comOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    this->pushFromPortEntry_comOut(data, context);
  }


} // end namespace Dtn
