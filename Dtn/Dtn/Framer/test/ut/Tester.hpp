// ======================================================================
// \title  Framer/test/ut/Tester.hpp
// \author root
// \brief  hpp file for Framer test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Dtn/Framer/Framer.hpp"

namespace Dtn {

  class Tester :
    public FramerGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:
      // Maximum size of histories storing events, telemetry, and port outputs
      static const NATIVE_INT_TYPE MAX_HISTORY_SIZE = 10;
      // Instance ID supplied to the component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_ID = 0;
      // Queue depth supplied to component instance under test
      static const NATIVE_INT_TYPE TEST_INSTANCE_QUEUE_DEPTH = 10;

      //! Construct object Tester
      //!
      Tester();

      //! Destroy object Tester
      //!
      ~Tester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! To do
      //!
      void toDo();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_dtnBufferOut
      //!
      void from_dtnBufferOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler for from_comStatus
      //!
      void from_comStatus_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Success &condition /*!< 
      Condition success/failure
      */
      );

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      Framer component;

  };

} // end namespace Dtn

#endif
