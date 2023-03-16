module Dtn {

  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {

    constant queueSize = 10

    constant stackSize = 64 * 1024

  }

  instance blockDrv: Drv.BlockDriver base id 0x0100 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 140

  instance framer: Svc.Framer base id 0x4200 {
    phase Fpp.ToCpp.Phases.configObjects """
    Svc::FprimeFraming framing;
    """

    phase Fpp.ToCpp.Phases.configComponents """
    framer.setup(ConfigObjects::framer::framing);
    """
  }

  # Each command/tlm/event is tagged with an ID so we're using 0x4200 + 0x20 here
  instance dtnFramer: Framer base id 0x4220 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 120

}
