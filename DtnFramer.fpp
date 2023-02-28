module Dtn {

  @ A component for framing input for transmission to the ground
  active component DtnFramer {

    # ----------------------------------------------------------------------
    # Receiving packets
    # ----------------------------------------------------------------------

    @ Port for receiving data packets of any type stored in statically-sized
    @ Fw::Com buffers
    guarded input port comIn: Fw.Com

    @ Port for receiving file packets stored in dynamically-sized
    @ Fw::Buffer objects
    guarded input port bufferIn: Fw.BufferSend

    # ----------------------------------------------------------------------
    # Allocation and deallocation of buffers
    # ----------------------------------------------------------------------

    # TODO

    # ----------------------------------------------------------------------
    # Sending frame data
    # ----------------------------------------------------------------------

    @ Fw::ComBuffer output port
    output port comQueueSend: Fw.Com

    @ Fw::Buffer output port
    output port buffQueueSend: Fw.BufferSend

    # ----------------------------------------------------------------------
    # Handling of of ready signals
    # ----------------------------------------------------------------------

    @ Port receiving the general status from the downstream component
    @ indicating it is ready or not-ready for more input
    guarded input port comStatusIn: Fw.SuccessCondition

    @ Port receiving indicating the status of framer for receiving more data
    output port comStatus: Fw.SuccessCondition

  }

}
