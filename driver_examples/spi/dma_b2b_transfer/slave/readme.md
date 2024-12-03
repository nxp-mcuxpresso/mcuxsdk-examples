# spi_dma_b2b_transfer_slave

## Overview
The spi_dma_board2board_slave example shows how to use spi driver as slave to do board to board transfer with DMA:

In this example, one spi instance as slave and another spi instance on other board as master. Master sends a piece of 
data to slave, and receive a piece of data from slave. This example checks if the data received from master is correct.

## Supported Boards
- [FRDM-K32L2B](../../../../_boards/frdmk32l2b/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC242](../../../../_boards/frdmmcxc242/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC444](../../../../_boards/frdmmcxc444/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../../_boards/twrkm34z50mv3/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z75M](../../../../_boards/twrkm34z75m/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM35Z75M](../../../../_boards/twrkm35z75m/driver_examples/spi/dma_b2b_transfer/slave/example_board_readme.md)
