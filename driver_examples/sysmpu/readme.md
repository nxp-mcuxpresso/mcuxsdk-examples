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

## Supported Boards
- [TWR-KM34Z50MV3](../../_boards/twrkm34z50mv3/driver_examples/sysmpu/example_board_readme.md)
- [TWR-KM34Z75M](../../_boards/twrkm34z75m/driver_examples/sysmpu/example_board_readme.md)
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/driver_examples/sysmpu/example_board_readme.md)
