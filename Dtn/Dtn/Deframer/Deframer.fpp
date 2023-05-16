module Dtn {

    @ A component for deframing input received from the ground
    @ via a byte stream driver, which may be active or passive
    active component Deframer {

        #
        # GENERAL PORTS
        #

        @ DTN buffer send in, serialized as DTN bundles.
        @ The buffer is assumed to be an uplinked file or command.
        @ Internally the component must check buffer metadata to make this determination.
        async input port bufferIn: Fw.BufferSend

        @ Port for receiving command responses from a command dispatcher.
        @ Invoking this port does nothing. The port exists to allow the matching
        @ connection in the topology.
        sync input port cmdResponseIn: Fw.CmdResponse

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
