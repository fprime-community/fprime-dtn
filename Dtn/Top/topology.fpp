module Dtn {

  topology Dtn {

    instance blockDrv
    instance comQueue
    instance framer
    instance dtnFramer

    connections Downlink {
      comQueue.comQueueSend -> dtnFramer.comIn
      comQueue.buffQueueSend -> dtnFramer.bufferIn
      dtnFramer.passthroughComOut -> framer.comIn
      dtnFramer.passthroughBufferOut -> framer.bufferIn
    }

  }

}
