// ======================================================================
// \title  DtnFramer/test/ut/TesterHelpers.cpp
// \author Auto-generated
// \brief  cpp file for DtnFramer component test harness base class
//
// NOTE: this file was automatically generated
//
// ======================================================================
#include "Tester.hpp"

namespace Com {
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

    // comIn
    this->connect_to_comIn(
        0,
        this->component.get_comIn_InputPort(0)
    );

    // comStatusIn
    this->connect_to_comStatusIn(
        0,
        this->component.get_comStatusIn_InputPort(0)
    );

    // comBundleOut
    this->component.set_comBundleOut_OutputPort(
        0,
        this->get_from_comBundleOut(0)
    );

    // comStatus
    this->component.set_comStatus_OutputPort(
        0,
        this->get_from_comStatus(0)
    );

    // passthroughBufferOut
    this->component.set_passthroughBufferOut_OutputPort(
        0,
        this->get_from_passthroughBufferOut(0)
    );

    // passthroughComOut
    this->component.set_passthroughComOut_OutputPort(
        0,
        this->get_from_passthroughComOut(0)
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

} // end namespace Com
