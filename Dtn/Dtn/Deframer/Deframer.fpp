module Dtn {

    @ A component for deframing input received from the ground
    @ via a byte stream driver, which may be active or passive
    active component Deframer {

        #
        # GENERAL PORTS
        #

        @ Buffer send in
        async input port bufferIn: Fw.BufferSend

        @ Port for sending passthrough command packets as Com buffers.
        @ This exists to retain existing functionality in case Svc.Deframer
        @ has a Com buffer to pass
        async input port passthroughComIn: Fw.Com

        #
        # SENDING COMMAND PACKETS AND RECEIVING COMMAND RESPONSES
        #

        @ Port for sending command packets as Com buffers.
        output port comOut: Fw.Com

        #
        # MEMORY MANAGEMENT FOR DEFRAMING AND FOR SENDING FILE PACKETS
        #

        @ Port for sending file packets (case 1 above).
        @ The file packets are wrapped in Fw::Buffer objects allocated with
        @ bufferAllocate.
        @ Ownership of the Fw::Buffer passes to the receiver, which is
        @ responsible for the deallocation.
        output port bufferOut: Fw.BufferSend

        #
        # BUFFER ALLOCATION & DEALLOCATION
        #

        # TODO
    }
}
