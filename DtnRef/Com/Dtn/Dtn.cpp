// ======================================================================
// \title  Dtn.cpp
// \author mstarch
// \brief  cpp file for Dtn component implementation class
// ======================================================================

#include <DtnRef/Com/Dtn/Dtn.hpp>
#include "Fw/Types/Assert.hpp"
#include "Fw/Types/BasicTypes.hpp"

extern "C" {
#include "bpchat.h"
}

namespace Com {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Dtn::Dtn(const char* const compName) : DtnComponentBase(compName), m_reinitialize(true) {}

void Dtn::init(const NATIVE_INT_TYPE instance) {
    DtnComponentBase::init(instance);
    printf("[DTN] bpchat starting\n");

    // Intentionally mutable strings instead of string literals.
    // ION's `parseEidString()` does not work for string literals
    char ownEid[] = "ipn:2.1";
    char destEid[] = "ipn:3.1";
    bpchat_start(ownEid, destEid);
}

Dtn::~Dtn() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

Drv::SendStatus Dtn::comDataIn_handler(const NATIVE_INT_TYPE portNum, Fw::Buffer& sendBuffer) {
    FW_ASSERT(!this->m_reinitialize);  // A message should never get here if we need to reinitialize is needed
    Drv::SendStatus driverStatus = Drv::SendStatus::SEND_RETRY;
    for (NATIVE_UINT_TYPE i = 0; driverStatus == Drv::SendStatus::SEND_RETRY && i < RETRY_LIMIT; i++) {
        driverStatus = this->drvDataOut_out(0, sendBuffer);
    }
    FW_ASSERT(driverStatus != Drv::SendStatus::SEND_RETRY);  // If it is still in retry state, there is no good answer
    Fw::Success comSuccess = (driverStatus.e == Drv::SendStatus::SEND_OK) ? Fw::Success::SUCCESS : Fw::Success::FAILURE;
    this->m_reinitialize = driverStatus.e != Drv::SendStatus::SEND_OK;
    if (this->isConnected_comStatus_OutputPort(0)) {
        this->comStatus_out(0, comSuccess);
    }
    printf("DTN\tcomDataIn_handler\n");
    return Drv::SendStatus::SEND_OK;  // Always send ok to deframer as it does not handle this anyway
}

void Dtn::drvConnected_handler(const NATIVE_INT_TYPE portNum) {
    Fw::Success radioSuccess = Fw::Success::SUCCESS;
    if (this->isConnected_comStatus_OutputPort(0) && m_reinitialize) {
        this->m_reinitialize = false;
        this->comStatus_out(0, radioSuccess);
    }
    printf("DTN\tdrvConnected_handler\n");
}

void Dtn::drvDataIn_handler(const NATIVE_INT_TYPE portNum,
                                Fw::Buffer& recvBuffer,
                                const Drv::RecvStatus& recvStatus) {
    printf("DTN\tcomDataOut_handler\n");
    this->comDataOut_out(0, recvBuffer, recvStatus);
}

}  // end namespace Com
