# flexspi_nand_polling_transfer

## Overview
The flexspi_nand_polling_transfer example shows how to use flexspi driver with polling:

In this example, flexspi will send data and operate the external Nand flash connected with FLEXSPI. Some simple flash command will
be executed, such as Write Enable, Erase block, Program page.
Example will first erase the block and program a page into the flash, at last check if the data in flash is correct.