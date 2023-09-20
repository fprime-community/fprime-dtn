# Quick Start

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
