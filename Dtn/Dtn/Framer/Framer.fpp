module Dtn {

    @ A component for framing input for transmission to the ground
    active component Framer {

        #
        # RECEIVING DATA TO FRAME
        #

        @ Port for receiving data packets of any type stored in statically-sized Fw::Com buffers
        async input port comIn: [2] Fw.Com drop # TODO use ComQueueBufferPorts

        @ Port for receiving file packets stored in dynamically-sized Fw::Buffer objects
        async input port bufferIn: [ComQueueBufferPorts] Fw.BufferSend drop

        #
        # SENDING FRAMED DATA
        #

        @ DTN buffer send out, serialized as LTP segments.
        @ Events, telemetry, and files are all be fed through this buffer.
        @ Deframing on the ground is assumed to check buffer metadata to determine type.
        output port bufferOut: Fw.BufferSend

        #
        # BUFFER ALLOCATION & DEALLOCATION
        #

        @ Port for deallocating buffers received on bufferIn, after
        @ copying packet data to the frame buffer
        output port bufferDeallocate: Fw.BufferSend

        @ Port for allocating buffers to hold framed data
        output port bufferAllocate: Fw.BufferGet

    }
}
