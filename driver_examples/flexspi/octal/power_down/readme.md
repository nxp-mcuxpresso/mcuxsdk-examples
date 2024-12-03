# flexspi_octal_power_down

## Overview
The flexspi_octal_power_down example shows how to use flexspi driver with polling and recover from power_down mode:

In this example, chip will first enter power_down and wakeup after 5s, then flexspi will send data and operate the external octal flash connected with FLEXSPI. Some simple flash command will be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/flexspi/octal/power_down/example_board_readme.md)
