# i2c_dma_b2b_transfer_slave

## Overview
The i2c_dma_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a DMA master:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct. I2C slave works as supporting board to help demonstrate master DMA trasnfer, actually uses interrupt
way.

## Supported Boards
- [FRDM-K32L2B](../../../../_boards/frdmk32l2b/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC242](../../../../_boards/frdmmcxc242/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC444](../../../../_boards/frdmmcxc444/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../../_boards/twrkm34z50mv3/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z75M](../../../../_boards/twrkm34z75m/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM35Z75M](../../../../_boards/twrkm35z75m/driver_examples/i2c/dma_b2b_transfer/slave/example_board_readme.md)
