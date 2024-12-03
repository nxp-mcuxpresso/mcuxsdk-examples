# qspi_dma_transfer

## Overview
The qspi_dma_transfer example shows how to use qspi driver with DMA:

In this example, qspi will send data and operate the external flash connected with QSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and programe a page into the flash, at last check if the data in flash is correct.
