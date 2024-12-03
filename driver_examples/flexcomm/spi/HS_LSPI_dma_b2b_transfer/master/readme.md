# spi_HS_LSPI_dma_b2b_transfer_master

## Overview
The spi_HS_LSPI_dma_b2b_transfer_master example shows how to use driver API to transfer in DMA way.  

In this example, one spi instance as master and another spi instance on the other board as slave. 
Master sends a piece of data to slave, and receive a piece of data from slave. This example checks
if the data received from slave is correct. This example needs to work with spi_HS_LSPI_dma_b2b_transfer_master example.

Note: This example will run in master mode, please prepare another board for slave, and the slave 
​      board should be started first.

## Project Infomation
1.How to set the baudrate of SPI?
  The baudrate of SPI is easy to set, just make sure the source clock of SPI.
  The formula is below:

  baudrate = PCLK_SPIn / DIVVAL
  SPIn->DIV = DIVVAL -1
  For example, if the needed baudrate of SPI is 500khz, and the SPI clock is 12MHz
  DIVVAL = 12000000/500000
  DIVVAL = 24
  SPIn->DIV = DIVVAL -1 = 23

Program Flow:
============= 
Main routine:
  1.Initialize the hardware.
​	Configure pin settings, clock settings by using BOARD_InitHardware();
​	
  2.Send information to terminal by using debug console.
​	
  3.Initialize the SPI with configuration.
​	
  4.Set up DMA configuration for master SPI.
​    Initialize DMA and DMA channel setting(create handle and set callback) for both
​    SPI RX and TX, set prioritory for TX channel and RX channel.
​	
  5.Start SPI master transfer in DMA way.
​    Initialize buffers and start SPI transfer in DMA way.
​	
  6.Check if data from master is all right.
​    Waiting for transmission complete, print received data and log information to terminal.

  7.De-initialize the SPI and DMA.

## Supported Boards
- [LPCXpresso55S06](../../../../../_boards/lpcxpresso55s06/driver_examples/spi/HS_LSPI_dma_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso55S16](../../../../../_boards/lpcxpresso55s16/driver_examples/spi/HS_LSPI_dma_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso55S28](../../../../../_boards/lpcxpresso55s28/driver_examples/spi/HS_LSPI_dma_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso55S36](../../../../../_boards/lpcxpresso55s36/driver_examples/spi/HS_LSPI_dma_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso55S69](../../../../../_boards/lpcxpresso55s69/driver_examples/spi/HS_LSPI_dma_b2b_transfer/master/example_board_readme.md)
