// ======================================================================
// \title  Deframer/test/ut/Tester.hpp
// \author root
// \brief  hpp file for Deframer test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "GTestBase.hpp"
#include "Dtn/Deframer/Deframer.hpp"

namespace Dtn {

  class Tester :
    public DeframerGTestBase
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

      //! Handler for from_bufferOut
      //!
      void from_bufferOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler for from_comOut
      //!
      void from_comOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::ComBuffer &data, /*!< 
      Buffer containing packet data
      */
          U32 context /*!< 
      Call context value; meaning chosen by user
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
      Deframer component;

  };

} // end namespace Dtn

#endif
