// ======================================================================
// \title  Dtn.hpp
// \author mstarch
// \brief  hpp file for Dtn component implementation class
// ======================================================================

#ifndef Com_Dtn_HPP
#define Com_Dtn_HPP

#include "DtnRef/Com/Dtn/DtnComponentAc.hpp"

namespace Com {

class Dtn : public DtnComponentBase {
  public:
    const NATIVE_UINT_TYPE RETRY_LIMIT = 10;
    // ----------------------------------------------------------------------
    // Construction, initialization, and destruction
    // ----------------------------------------------------------------------

    //! Construct object Dtn
    //!
    Dtn(const char* const compName /*!< The component name*/
    );

    //! Initialize object Dtn
    //!
    void init(const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
    );

    //! Destroy object Dtn
    //!
    ~Dtn() override;

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
