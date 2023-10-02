# `fprime-dtn`

In this guide we will discuss running `fprime-dtn` along with an implementation overview of uplink and downlink.

## Running

The quickest way to run an F´ deployment with DTN components and a DTN-enabled GDS is to run:
```
./run
```
See `./run -h` for all command-line options:
```
usage: run [-h] [--fsw-entrypoint FSW_ENTRYPOINT]
           [--gds-entrypoint GDS_ENTRYPOINT] [--gds-attached]

Run F' DTN integration container.

optional arguments:
  -h, --help            show this help message and exit
  --fsw-entrypoint FSW_ENTRYPOINT
                        (default: entrypoint/deployment-entrypoint 10.0.0.2
                        10.0.0.3 ; bash)
  --gds-entrypoint GDS_ENTRYPOINT
                        (default: entrypoint/gds-entrypoint 10.0.0.3 10.0.0.2
                        ; bash)
  --gds-attached        (default: False)
```

This will spawn two containers: `fprime-gds` and `fprime-dtn`.
The GDS container runs the F´ GDS, hosted on the localhost.
End-to-end communication through DTN for all communication (including file uplink and downlink) is used.

To run an automated integration test run:
```
./run-it
```
The automated integration test starts both the reference deployment and GDS, programmatically uplinks a number of FSW
commands, and asserts that an expected number and volume of bundles were transmitted and received through ION –
verifying that DTN is being used end-to-end.

## Overview

The [`fprime-dtn`](https://github.com/fprime-community/fprime-dtn) project consists of two main DTN-enabled
elements developed in C++: a reference project and a separate component library.  The reference project is
located within `Ref/` and features a flight-like topology that is based on the F Prime System Reference project,
providing a template for real-world applications.  The component library is located within `Dtn/` and is engineered
to function as an external library for F Prime projects, facilitating integration and customization.  The project is
compatible with the latest F Prime version, v3.3.2.

Statically linked ION libraries used within `fprime-dtn` are generated from `ion-core`.  The
[`ion-core`](https://github.com/nasa-jpl/ion-core) project is a streamlined version of the ION open-source
software suite and offers essential DTN functionalities including BP, LTP, UDP convergence layer adapters, bundle
store-and-forward capabilities, hop-by-hop reliability, and routing mechanisms

The GDS used to demonstrate end-to-end DTN communication is implemented within the
[`fprime-gds-dtn`](https://github.com/pcrosemurgy/fprime-gds-dtn) project as a fork of
[`fprime-gds`](https://github.com/fprime-community/fprime-gds). For ease of deployment and operation,
Debian-based Docker images are defined to reproducibly build and run containerized F Prime DTN deployments with the
DTN-enabled F Prime GDS.

The F Prime DTN framer component and deframing protocol "infrastructure" both interact with ION DTN services by
"attaching" to the shared memory and semaphores that facilitate ION's interprocess communications (IPC).  This enables
a suite of ION daemons to operate in the background, performing various tasks such as buffering, data processing, and
routing on behalf of the DTN infrastructure. To ensure that the DTN infrastructure is the only FSW that has access to
ION we intentionally only build this source with statically linked ION libraries as opposed to building all of FSW with
the linked libraries.  Helper classes are also used to keep the framer component and deframing protocol implementations
minimal and to isolate direct calls to ION (`Dtn::FramerHelper` and `Dtn::DeframerHelper` respectively).

### Downlink

The downlink `dtnFramer` component is a passive component with an inner thread.  The input port handlers for telemetry
and events (`comIn`) and file downlink (`bufferIn`) ingest the application-layer data into ION through the non-blocking
`bp_send()` ION function. An inner thread calls the blocking `ltpDequeueOutboundSegment()` ION function in a continuous
loop to pass LTP segments downstream to the stock F Prime framing topology.

The only output port being used to flow data downstream is the `bufferOut` port. Since all application-layer data is
converted to bundles and segmented by LTP, an arbitrarily-sized buffer from a pre-allocated region of memory is needed
to store segments. When the stock `framer` frames each segment buffer it automatically uses the `FW_PACKET_FILE` packet
type which supports arbitrarily-sized data.

### Uplink

From a topological perspective, DTN-enabled uplink does not require any changes to the standard F Prime uplink topology.
This is due to deframing and routing logic being implemented internally within the `deframer` component's deframing
protocol implementation.  Our `Dtn::DeframingProtocol` acts as an intermediary between the stock F Prime deframer
and deframing protocol and is enabled through the project's `Ref/Top/instances.fpp` phase configuration for the
`deframer` component.

Similarly to the downlink component implementation, the `Dtn::DeframingProtocol` also contains an inner thread to
continuously invoke blocking `bp_receive()` calls to obtain uplinked data (either a command or an uplinked file).  A
complete bundle will only be returned by ION once all LTP segments comprising the bundle are ingested with
`ltpHandleInboundSegment()`. LTP segments are ingested after an initial deframing step is taken to strip the F Prime
framing data from the LTP segment.  Once a bundle is obtained from ION, `Dtn::DeframingProtocol` calls `route()` to have
the stock `deframer` pass the deframed data command or file to the appropriate component.

See `Dtn/Dtn/DeframingProtocol/` for the complete deframing protocol implementation.

### Ground Data System

The `fprime-gds-dtn` project was developed as a fork of `fprime-gds` with a minimal set of changes required to integrate
ION within the GDS. The most pertinent change is the addition of a `DtnDownlinker` and `DtnUplinker` within `updown.py`.
These two new classes mirror the existing `Downlinker` and `Uplinker`. With these changes being as isolated as they are
it will be possible to deliver this GDS DTN implementation as a GDS plugin when a thin plugin architecture for
downlink/uplink implementations is supported.  With the F Prime GDS being implemented in Python, the
[`pyion`](https://github.com/msancheznet/pyion) project is used to invoke ION through an established and tested Foreign
function interface (FFI). The `pyion` wrapper maintains a near one-to-one mapping of the ION API, resulting in a GDS DTN
implementation for uplink and downlink that appears very similar to the flight implementations.

`DtnDownlinker` is functionally similarly to the flight uplink implementation. Data received on the ground is deframed
by the GDS checking for the presence of an F Prime "start token". Each deframed packet is ingested into ION using
`pyion`'s `ltp_handle_inbound_segment()`, while on a separate thread `bp_receive()` is called to obtain a deframed
telemetry, event, or file packet.

`DtnUplinker` is functionally similarly to the flight downlink implementation. A command or file to be uplinked to a
spacecraft is framed by first ingesting the ADU into ION through `bp_send()`, while on a separate thread
`ltp_dequeue_outbound_segment()` is called to return LTP segments to pass along to the stock GDS framing implementation.

No changes were needed to the GDS graphical user interface (GUI) which is accessed through a self-hosted web server.
File uplink and downlink both function as expected as the GUI is agnostic to implementation details of the underlying
communications stack.

## Future Work

As development progresses, the following limitations and desired features should receive attention: 
1. The current prototype is designed to operate with a single underlying communication service – such as a
    mission-specific framing and radio stack – future iterations are planned to incorporate "multiduct"
    capabilities to support the simultaneous operation of multiple radio and link-layer protocols.
1. ADU delivery status verification is not currently provided. Limited bundle status reporting capabilities may
    be included in future releases.
1. Load-and-go command support for tactical ION configuration changes.
1. Deployment and testing on flight hardware.
1. `Os::Task` should be used instead of `pthread.h`.
1. File uplink and downlink appear to run slower than expected.
    - Some tests should be done to measure a baseline for file uplink/downlink speeds on a "stock" F Prime System Reference.
1. A plugin infrastructure (using `pluggy`) should be implemented in the core `fprime-gds` repository.
    - This would allow for the DTN uplinker/downlinker to exist as a simpler plugin instead of a full fork of
    `fprime-gds`.
1. Review and merge https://github.com/nasa/fprime/pull/2150 to decouple routing from deframing.
