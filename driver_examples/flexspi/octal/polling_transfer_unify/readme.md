# flexspi_octal_polling_transfer_unify

## Overview
The flexspi_octal_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external octal flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.

## Supported Boards
- [EVK9-MIMX8ULP](../../../../_boards/evk9mimx8ulp/driver_examples/flexspi/octal/polling_transfer_unify/example_board_readme.md)
- [EVK-MIMX8ULP](../../../../_boards/evkmimx8ulp/driver_examples/flexspi/octal/polling_transfer_unify/example_board_readme.md)
