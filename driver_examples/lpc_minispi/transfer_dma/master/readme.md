# spi_transfer_dma_master

## Overview
The spi_dma_transfer_master example shows how to use driver API to transfer in DMA way.  

In this example, one spi instance as master and another spi instance on the other board as slave. 
Master sends a piece of data to slave, and receive a piece of data from slave. This example checks
if the data received from slave is correct. This example needs to work with spi_dma_transfer_slave 
example.

## Supported Boards
- [LPC845BREAKOUT](../../../../_boards/lpc845breakout/driver_examples/spi/transfer_dma/master/example_board_readme.md)
- [LPCXpresso824MAX](../../../../_boards/lpcxpresso824max/driver_examples/spi/transfer_dma/master/example_board_readme.md)
- [LPCXpresso845MAX](../../../../_boards/lpcxpresso845max/driver_examples/spi/transfer_dma/master/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/spi/transfer_dma/master/example_board_readme.md)
