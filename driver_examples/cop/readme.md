# cop

## Overview
The COP Example project is to demonstrate usage of the KSDK cop driver.
In this example, after 10 times of refreshing, a timeout reset is generated.
Please notice that because COP control registers are write-once only, so the 
COP_Init function and the COP_Disable function can be called only once after reset.
In SystemInit() function which is called in startup code, there is an operation to 
disable the watchdog if macro DISABLE_WDOG is defined as 0, and by default if the 
DISABLE_WDOG is not defined, the DISABLE_WDOG is defined as 1. So this example has 
a special project setting that defines the DISABLE_WDOG macro to 0 so the startup 
code will skip the disable COP WDOG operation.

## Supported Boards
- [FRDM-K32L2B](../../_boards/frdmk32l2b/driver_examples/cop/example_board_readme.md)
- [FRDM-MCXC041](../../_boards/frdmmcxc041/driver_examples/cop/example_board_readme.md)
- [FRDM-MCXC242](../../_boards/frdmmcxc242/driver_examples/cop/example_board_readme.md)
- [FRDM-MCXC444](../../_boards/frdmmcxc444/driver_examples/cop/example_board_readme.md)
