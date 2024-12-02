# lpc_i2c_dma_b2b_master

## Overview
The i2c_dma_b2b_transfer_master example shows how to use i2c driver as master to do board to board transfer 
with DMA:

In this example, one i2c instance as master and another i2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [LPCXpresso824MAX](../../../../_boards/lpcxpresso824max/driver_examples/i2c/dma_b2b/master/example_board_readme.md)
- [LPCXpresso845MAX](../../../../_boards/lpcxpresso845max/driver_examples/i2c/dma_b2b/master/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i2c/dma_b2b/master/example_board_readme.md)
