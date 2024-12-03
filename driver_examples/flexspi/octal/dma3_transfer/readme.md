# flexspi_octal_dma3_transfer

## Overview
The flexspi_nor_edma_transfer example shows how to use flexspi driver with dma:

In this example, flexspi will send data and operate the external nor flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/flexspi/octal/edma_transfer/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/flexspi/octal/edma_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/flexspi/octal/edma_transfer/example_board_readme.md)
