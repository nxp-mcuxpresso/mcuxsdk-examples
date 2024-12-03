# i2c_dma_b2b_transfer_master

## Overview
The i2c_dma_b2b_transfer_master example shows how to use i2c driver as master to do board to board transfer 
with DMA:

In this example, one i2c instance as master and another i2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [FRDM-K32L2B](../../../../_boards/frdmk32l2b/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXC242](../../../../_boards/frdmmcxc242/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXC444](../../../../_boards/frdmmcxc444/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../../_boards/twrkm34z50mv3/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
- [TWR-KM34Z75M](../../../../_boards/twrkm34z75m/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
- [TWR-KM35Z75M](../../../../_boards/twrkm35z75m/driver_examples/i2c/dma_b2b_transfer/master/example_board_readme.md)
