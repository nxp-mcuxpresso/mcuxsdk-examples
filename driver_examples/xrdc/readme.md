# xrdc

## Overview
The xrdc example shows how to control the memory and peripheral access
policy using XRDC.

In this example, one peripheral and a memory region are set to unaccessible, then
the hardfault occurs.

Running the demo
================
The log below shows the output of the example in the terminal window.
**NOTE** The access address might be different by platforms.
```
XRDC example start
Set the peripheral not accessible
Violent access at address: 0x40032000
The peripheral is accessible now
Set the memory not accessible
Violent access at address: 0x08000000
The memory is accessible now
XRDC example Success
```

## Supported Boards
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/driver_examples/xrdc/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/driver_examples/xrdc/example_board_readme.md)
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/driver_examples/xrdc/example_board_readme.md)
