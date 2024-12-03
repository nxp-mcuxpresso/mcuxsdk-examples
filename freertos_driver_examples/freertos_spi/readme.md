# freertos_spi

## Overview
The freertos_spi example shows how to use SPI driver in FreeRTOS:

In this example , one spi instance is used as SPI master with blocking and another spi instance is used as SPI slave.

1. SPI master sends/receives data using task blocking calls to/from SPI slave. (SPI Slave uses interrupt to receive/send
the data)

## Running the demo
When the example runs successfully, you can see the similar information on the terminal as below.

SPI FreeRTOS example start.
This example use one SPI instance in master mode
to transfer data through loopback
Please be sure to externally connect together SOUT and SIN signals.
   SOUT     --    SIN  
SPI transfer completed successfully. 
Data verified ok.

## Supported Boards
- [FRDM-K32L2B](../../_boards/frdmk32l2b/freertos_driver_examples/freertos_spi/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/freertos_driver_examples/freertos_spi/example_board_readme.md)
