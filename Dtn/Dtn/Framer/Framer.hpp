// ======================================================================
// \title  Framer.hpp
// \author root
// \brief  hpp file for DTN Framer component implementation class
// ======================================================================

#ifndef Dtn_Framer_HPP
#define Dtn_Framer_HPP

#include "Dtn/Framer/FramerComponentAc.hpp"

namespace Dtn {

  class Framer :
    public FramerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object Framer
      //!
      Framer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object Framer
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object Framer
      //!
      ~Framer();

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

      //! Handler implementation for comIn
      //!
      void comIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::ComBuffer &data, /*!< 
      Buffer containing packet data
      */
          U32 context /*!< 
      Call context value; meaning chosen by user
      */
      );

      //! Handler implementation for comStatusIn
      //!
      void comStatusIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Success &condition /*!< 
      Condition success/failure
      */
      );


    };

} // end namespace Dtn

#endif // Dtn_Framer_HPP
