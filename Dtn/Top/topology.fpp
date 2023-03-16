module Dtn {

  topology Dtn {

    instance framer
    instance dtnFramer

    connections Downlink {
      dtnFramer.passthroughComOut -> framer.comIn
      dtnFramer.passthroughBufferOut -> framer.bufferIn
    }

  }

}
