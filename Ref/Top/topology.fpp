module Ref {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

  enum Ports_RateGroups {
    rateGroup1
    rateGroup2
    rateGroup3
  }

  enum Ports_StaticMemory {
    downlink
    uplink
  }

  topology Ref {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance $health
    instance blockDrv
    instance chanTlm
    instance cmdDisp
    instance cmdSeq
    instance comDriver
    instance dtnFramer
    instance comQueue
    instance radio
    instance framer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance fileDownlink
    instance fileManager
    instance fileUplink
    instance comBufferManager
    instance linuxTime
    instance prmDb
    instance rateGroup1Comp
    instance rateGroup2Comp
    instance rateGroup3Comp
    instance rateGroupDriverComp
    instance textLogger
    instance deframer
    instance systemResources
    instance imu
    instance imuI2cBus
    instance camera
    instance saveImageBufferLogger
    instance imageProcessor
    instance processedImageBufferLogger

    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    param connections instance prmDb

    telemetry connections instance chanTlm

    text event connections instance textLogger

    time connections instance linuxTime

    health connections instance $health

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections Downlink {
      chanTlm.PktSend -> dtnFramer.comIn
      eventLogger.PktSend -> dtnFramer.comIn
      fileDownlink.bufferSendOut -> dtnFramer.bufferIn

      # Only Fw::Buffer types are sent, Fw::Com sizing is too restrictive for DTN data
      dtnFramer.bufferOut -> comQueue.buffQueueIn[0]

      # TODO where does deallocation of Fw::Com happen?
      dtnFramer.bufferDeallocate -> fileDownlink.bufferReturn
      dtnFramer.bufferAllocate -> comBufferManager.bufferGetCallee

      comQueue.comQueueSend -> framer.comIn
      comQueue.buffQueueSend -> framer.bufferIn

      framer.framedOut -> radio.comDataIn
      framer.framedAllocate -> comBufferManager.bufferGetCallee
      framer.bufferDeallocate -> comBufferManager.bufferSendIn
      comDriver.deallocate -> comBufferManager.bufferSendIn

      radio.drvDataOut -> comDriver.send
      comDriver.ready -> radio.drvConnected
      radio.comStatus -> framer.comStatusIn
      framer.comStatusOut -> comQueue.comStatusIn
    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections RateGroups {
      # Block driver
      blockDrv.CycleOut -> rateGroupDriverComp.CycleIn

      # Rate group 1
      rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1Comp.CycleIn
      rateGroup1Comp.RateGroupMemberOut[0] -> chanTlm.Run
      rateGroup1Comp.RateGroupMemberOut[1] -> fileDownlink.Run
      rateGroup1Comp.RateGroupMemberOut[2] -> systemResources.run
      rateGroup1Comp.RateGroupMemberOut[3] -> imu.Run
# Commented out in F' SystemReference:
#      rateGroup1Comp.RateGroupMemberOut[4] -> radio.run

      # Rate group 2
      rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2Comp.CycleIn
      rateGroup2Comp.RateGroupMemberOut[0] -> cmdSeq.schedIn

      # Rate group 3
      rateGroupDriverComp.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3Comp.CycleIn
      rateGroup3Comp.RateGroupMemberOut[0] -> $health.Run
      rateGroup3Comp.RateGroupMemberOut[1] -> blockDrv.Sched
      rateGroup3Comp.RateGroupMemberOut[2] -> comBufferManager.schedIn
    }

    connections Sequencer {
      cmdSeq.comCmdOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> cmdSeq.cmdResponseIn
    }

    connections Uplink {
      comDriver.allocate -> comBufferManager.bufferGetCallee
      comDriver.$recv -> radio.drvDataIn

      radio.comDataOut -> deframer.framedIn
      deframer.framedDeallocate -> comBufferManager.bufferSendIn

      deframer.bufferOut   -> fileUplink.bufferSendIn
      deframer.comOut      -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> comBufferManager.bufferGetCallee
      deframer.bufferDeallocate -> comBufferManager.bufferSendIn
      fileUplink.bufferSendOut -> comBufferManager.bufferSendIn
    }

# Commented out in F' SystemReference:
#    connections Radio {
#      radio.allocate -> comBufferManager.bufferGetCallee
#      radio.deallocate -> comBufferManager.bufferSendIn
#    }

    connections I2c {
      imu.read -> imuI2cBus.read
      imu.write -> imuI2cBus.write
    }

    connections Camera {
       camera.allocate -> comBufferManager.bufferGetCallee
       camera.deallocate -> comBufferManager.bufferSendIn
       camera.$save -> saveImageBufferLogger.bufferSendIn
       saveImageBufferLogger.bufferSendOut -> comBufferManager.bufferSendIn

       camera.process->imageProcessor.imageData
       imageProcessor.postProcess -> processedImageBufferLogger.bufferSendIn
       imageProcessor.bufferAllocate -> comBufferManager.bufferGetCallee
       imageProcessor.bufferDeallocate -> comBufferManager.bufferSendIn
       processedImageBufferLogger.bufferSendOut -> comBufferManager.bufferSendIn
    }

  }

}
