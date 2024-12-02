# xspi_nor_polling_transfer

## Overview
The xspi_nor_polling_transfer example shows how to use xspi driver with polling:

In this example, xspi will send data and operate the external Nor flash connected with XSPI. Some simple flash command
will be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.
