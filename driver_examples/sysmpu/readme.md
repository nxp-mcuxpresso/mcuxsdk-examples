# sysmpu

## Overview

The SYSMPU example defines protected/unprotected memory region for the core access.

First, the SYSMPU will capture the hardware information and show it on the
terminal. Then, a memory region is configured as the non-writable region. The
bus fault interrupt is enabled to report the memory protection interrupt event
for this non-writable region. If an operation writes to this region, the bus
fault interrupt happens. Then the bus fault interrupt handler provides a
prevention alert by outputting a message on terminal, then the write rights
will be given to this region for core access. After the write access enabled,
the writing to the region becomes successful. When the bus fault happen, the
details of the error information will be captured and printed on the terminal.

This example provides the terminal input control to give the example show for 
several regions access test. Just press any key to the terminal when the
terminal show "Press any key to continue".

## Running the demo

When the demo runs successfully, the following message is displayed in the terminal:
```
SYSMPU Example Start.

SYSMPU revision level 1, 5 slave ports and 12 region numbers are supported.

Set regionArray0 to un-writeable.

Write 0 to regionArray at No.0.

Core access violation and generate busfault.

RegionArray[0] = 0, not updated.

Core is granted write access permission.

Error access happen on slave port: 2, access master port 0.

Error access attributes: accessSupervisorMode.

Error access type: write.

Error access control: overlappRegion.

Error access address: xxxxxxx.

RegionArray[0] = 0, updated now.

Protected regionArray successfully.

Press any key to continue.
```

Otherwise, the log will show
```
ERROR: Cannot protect regionArray.
```

## Supported Boards
- [TWR-KM34Z50MV3](../../_boards/twrkm34z50mv3/driver_examples/sysmpu/example_board_readme.md)
- [TWR-KM34Z75M](../../_boards/twrkm34z75m/driver_examples/sysmpu/example_board_readme.md)
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/driver_examples/sysmpu/example_board_readme.md)
- [FRDM-MCXE247](../../_boards/frdmmcxe247/driver_examples/sysmpu/example_board_readme.md)
