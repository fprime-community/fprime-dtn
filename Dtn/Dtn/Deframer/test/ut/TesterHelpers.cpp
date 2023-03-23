// ======================================================================
// \title  Deframer/test/ut/TesterHelpers.cpp
// \author Auto-generated
// \brief  cpp file for Deframer component test harness base class
//
// NOTE: this file was automatically generated
//
// ======================================================================
#include "Tester.hpp"

namespace Dtn {
  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

  void Tester ::
    connectPorts()
  {

    // bufferIn
    this->connect_to_bufferIn(
        0,
        this->component.get_bufferIn_InputPort(0)
    );

    // passthroughComIn
    this->connect_to_passthroughComIn(
        0,
        this->component.get_passthroughComIn_InputPort(0)
    );

    // bufferOut
    this->component.set_bufferOut_OutputPort(
        0,
        this->get_from_bufferOut(0)
    );

    // comOut
    this->component.set_comOut_OutputPort(
        0,
        this->get_from_comOut(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        Tester::TEST_INSTANCE_QUEUE_DEPTH, Tester::TEST_INSTANCE_ID
    );
  }

} // end namespace Dtn
