# xspi_nor_edma_transfer

## Overview
The xspi_nor_edma_transfer example shows how to use xspi driver with edma:

In this example, xspi will send data and operate the external nor flash connected with XSPI. Some simple flash command
will be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and program a page into the flash, at last check if the data in flash is correct.
