// ======================================================================
// \title  DtnFramer.hpp
// \author mstarch
// \brief  hpp file for DtnFramer component implementation class
// ======================================================================

#ifndef Com_DtnFramer_HPP
#define Com_DtnFramer_HPP

#include "DtnRef/Com/Dtn/DtnFramer/DtnFramerComponentAc.hpp"

namespace Com {

class DtnFramer : public DtnFramerComponentBase {
  public:
    const NATIVE_UINT_TYPE RETRY_LIMIT = 10;
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object DtnFramer
    //!
    DtnFramer(const char* const compName /*!< The component name*/
    );

    //! Initialize object DtnFramer
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy object DtnFramer
    //!
    ~DtnFramer() override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for comDataIn
    //!
    Drv::SendStatus comDataIn_handler(const NATIVE_INT_TYPE portNum, /*!< The port number*/
                                      Fw::Buffer& sendBuffer) override;

    //! Handler implementation for drvConnected
    //!
    void drvConnected_handler(const NATIVE_INT_TYPE portNum) override;

    //! Handler implementation for drvDataIn
    //!
    void drvDataIn_handler(const NATIVE_INT_TYPE portNum,
                           /*!< The port number*/ Fw::Buffer& recvBuffer,
                           const Drv::RecvStatus& recvStatus) override;

    bool m_reinitialize;  //!< Stores if a ready signal is needed on connection
};

}  // end namespace Com

#endif
