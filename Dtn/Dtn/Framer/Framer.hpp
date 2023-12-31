// ======================================================================
// \title  Framer.hpp
// \author root
// \brief  hpp file for DTN Framer component implementation class
// ======================================================================

#ifndef Dtn_Framer_HPP
#define Dtn_Framer_HPP

#include "Dtn/Framer/FramerComponentAc.hpp"
#include "FramerHelper.hpp"
#include <pthread.h>

namespace Dtn
{

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

      void configure(pthread_mutex_t& sdrMutex);

      void start();

      //! Destroy object Framer
      //!
      ~Framer();

    PRIVATE:

      FramerHelper helper;

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


    };

} // end namespace Dtn

#endif // Dtn_Framer_HPP
