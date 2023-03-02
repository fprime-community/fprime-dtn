module Com
{
    @ A component for framing input for transmission to the ground
    active component DtnFramer
    {
        #
        # RECEIVING PACKETS
        #

        @ Port for receiving data packets of any type stored in statically-sized Fw::Com buffers
        async input port comIn: Fw.Com

        @ Port for receiving file packets stored in dynamically-sized Fw::Buffer objects
        async input port bufferIn: Fw.BufferSend

        #
        # BUFFER ALLOCATION & DEALLOCATION
        #

        # TODO

        #
        # SENDING FRAME DATA
        #

        #output port comQueueSend: Fw.Com

        #output port buffQueueSend: Fw.BufferSend

        output port comBundleOut: Fw.BufferSend

        #
        # READY SIGNALS
        #

        @ Port receiving the general status from the downstream component
        @ indicating it is ready or not-ready for more input
        guarded input port comStatusIn: Fw.SuccessCondition

        @ Port receiving indicating the status of framer for receiving more data
        output port comStatus: Fw.SuccessCondition
    }
}
