# qspi_edma_transfer

## Overview
The qspi_edma example shows how to use qspi driver with EDMA:

In this example, qspi will send data and operate the external flash connected with QSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and programe a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/qspi/edma_transfer/example_board_readme.md)
