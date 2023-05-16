// ======================================================================
// \title  Deframer.hpp
// \author root
// \brief  hpp file for Deframer component implementation class
// ======================================================================

#ifndef Dtn_Deframer_HPP
#define Dtn_Deframer_HPP

#include "Dtn/Deframer/DeframerComponentAc.hpp"

namespace Dtn
{

  class Deframer :
    public DeframerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Deframer
      //!
      Deframer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Deframer
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object Deframer
      //!
      ~Deframer();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for bufferIn
      //!
      void bufferIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for cmdResponseIn
      //!
      void cmdResponseIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          FwOpcodeType opCode, /*!< 
      Command Op Code
      */
          U32 cmdSeq, /*!< 
      Command Sequence
      */
          const Fw::CmdResponse &response /*!< 
      The command response argument
      */
      );

    };

} // end namespace Dtn

#endif // Dtn_Deframer_HPP
