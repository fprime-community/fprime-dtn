module Dtn {

    @ A component for framing input for transmission to the ground
    passive component Framer {

        #
        # RECEIVING DATA TO FRAME
        #

        @ Port for receiving data packets of any type stored in statically-sized Fw::Com buffers
        guarded input port comIn: Fw.Com

        @ Port for receiving file packets stored in dynamically-sized Fw::Buffer objects
        guarded input port bufferIn: Fw.BufferSend

        #
        # SENDING FRAMED DATA
        #

        @ DTN buffer send out, serialized as LTP segments.
        @ Events, telemetry, and files are all fed through this buffer.
        @ Deframing on the ground is assumed to check buffer metadata to determine type
        output port bufferOut: Fw.BufferSend

        #
        # BUFFER ALLOCATION & DEALLOCATION
        #

        @ Port for allocating buffers to hold framed data
        output port bufferAllocate: Fw.BufferGet

        @ Port for deallocating buffers intended to be created and destroyed
        @ only within this component
        output port bufferDeallocate: Fw.BufferSend

        @ Port for deallocating buffers received on bufferIn, after
        @ copying packet data to the frame buffer
        output port fileBufferDeallocate: Fw.BufferSend
    }
}
