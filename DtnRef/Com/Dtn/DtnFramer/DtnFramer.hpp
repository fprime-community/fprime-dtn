// ======================================================================
// \title  DtnFramer.hpp
// \author root
// \brief  hpp file for DtnFramer component implementation class
// ======================================================================

#ifndef DtnFramer_HPP
#define DtnFramer_HPP

#include "DtnRef/Com/Dtn/DtnFramer/DtnFramerComponentAc.hpp"

namespace Com {

  class DtnFramer :
    public DtnFramerComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object DtnFramer
      //!
      DtnFramer(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object DtnFramer
      //!
      void init(
          const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object DtnFramer
      //!
      ~DtnFramer();

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

} // end namespace Com

#endif
