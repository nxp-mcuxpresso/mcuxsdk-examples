# lpc_i2c_dma_b2b_slave

## Overview
The i2c_dma_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a DMA master:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct. I2C slave works as supporting board to help demonstrate master DMA trasnfer, actually uses interrupt
way.

## Supported Boards
- [LPCXpresso824MAX](../../../../_boards/lpcxpresso824max/driver_examples/i2c/dma_b2b/slave/example_board_readme.md)
- [LPCXpresso845MAX](../../../../_boards/lpcxpresso845max/driver_examples/i2c/dma_b2b/slave/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i2c/dma_b2b/slave/example_board_readme.md)
