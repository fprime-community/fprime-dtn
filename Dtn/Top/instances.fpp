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

  instance comQueue: Svc.ComQueue base id 0x0600 \
      queue size 200 \
      stack size Default.stackSize \
      priority 100 \
  {
    phase Fpp.ToCpp.Phases.configObjects """
    Svc::ComQueue::QueueConfigurationTable configurationTable;
    """

    phase Fpp.ToCpp.Phases.configComponents """
    // Channels, deep queue, low priority
    ConfigObjects::comQueue::configurationTable.entries[0] = {.depth = 500, .priority = 2};
    // Events , highest-priority
    ConfigObjects::comQueue::configurationTable.entries[1] = {.depth = 100, .priority = 0};
    // File Downlink
    ConfigObjects::comQueue::configurationTable.entries[2] = {.depth = 100, .priority = 1};
    // Allocation identifier is 0 as the MallocAllocator discards it
    comQueue.configure(ConfigObjects::comQueue::configurationTable, 0, Allocation::mallocator);
    """
  }

  // TODO getting a `Svc.Framer` and `Dtn.Framer` name collision when compiling.
  #instance framer: Svc.Framer base id 0x4200 {
  #  phase Fpp.ToCpp.Phases.configObjects """
  #  Svc::FprimeFraming framing;
  #  """

  #  phase Fpp.ToCpp.Phases.configComponents """
  #  framer.setup(ConfigObjects::framer::framing);
  #  """
  #}

  # Each command/tlm/event is tagged with an ID so we're using 0x4200 + 0x20 here
  instance dtnFramer: Dtn.Framer base id 0x4220 \
    queue size Default.queueSize \
    stack size Default.stackSize \
    priority 120

}
