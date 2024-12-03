# niobe4analog_flexspi_nor

## Overview

The FLASIAP NOR project is a simple demonstration program of the SDK FLASIAP NOR
driver. It erases and programs a portion of external Nor flash connected with
FLEXSPI. Some simple flash command willbe executed,such as Write Enable, Erase
sector, Program page. Example will first erase the sector and program a page
into the flash, at last check if the data in flash is correct.

## Supported Boards
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/flexspi_nor/example_board_readme.md)
