module Dtn
{
    @ A component for deframing input received from the ground
    @ via a byte stream driver, which may be active or passive
    active component Deframer
    {
        #
        # GENERAL PORTS
        #

        # TODO this is not an arrayed port like `CmdDispatcher.seqCmdBuff` because
        # we only get a single `Fw.Com` from `Deframer.comOut`. Is this okay?
        @ Command buffer input port for sequencers or other sources of command buffers
        #async input port seqCmdBuff: Fw.Com

        @ Buffer send in
        async input port bufferSendIn: Fw.BufferSend

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
