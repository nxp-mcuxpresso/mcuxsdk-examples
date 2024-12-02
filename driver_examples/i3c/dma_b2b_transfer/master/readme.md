# i3c_dma_b2b_transfer_master

## Overview
The example shows how to use i3c driver as master to do board to board transfer using dma method. 

In this example, one i3c instance as master and another i3c instance on the other board as slave.
Master first sends a piece of data to slave in I2C mode, and receive a piece of data from slave
and check if the data received from slave is correct. Then master will enter dynamic address cycle
to assign address to the connected slave, after success, will use I3C SDR mode to transfer data, then
receive data from the connected I3C slave and check the data consistency.

## Supported Boards
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/i3c/dma_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i3c/dma_b2b_transfer/master/example_board_readme.md)
