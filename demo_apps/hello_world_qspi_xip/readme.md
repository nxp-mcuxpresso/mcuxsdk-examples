# hello_world_qspi_xip

## Overview
The Hello World demo application provides a sanity check for the new SDK build environments and board bring up. The Hello
World demo prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to
show how to use the UART, and to provide a simple project for debugging and further development.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.

## Supported Boards
- [FRDM-MCXN947](../../_boards/frdmmcxn947/demo_apps/hello_world_qspi_xip/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/demo_apps/hello_world_qspi_xip/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/demo_apps/hello_world_qspi_xip/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/demo_apps/hello_world_qspi_xip/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/demo_apps/hello_world_qspi_xip/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/demo_apps/hello_world_qspi_xip/example_board_readme.md)
