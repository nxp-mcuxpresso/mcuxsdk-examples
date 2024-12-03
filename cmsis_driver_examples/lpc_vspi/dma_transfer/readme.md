# cmsis_spi_dma_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_dma_transfer example shows how to use CMSIS SPI driver in dma way:
In this example , one spi instance used as master and another spi instance used as slave in the same board.
SPI master send/received data to/from SPI slave in dma . (SPI Slave using dma to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.
SPI CMSIS DMA transfer example start.
This example use one spi instance as master and another as slave on one board.
Master uses DMA way , slave uses DMA way too.
Please make sure you make the correct line connection. Basically, the connection is:
SPI_master  --  SPI_slave
   CLK      --    CLK
   PCS      --    PCS
   MISO     --    MISO
   MOSI     --    MOSI
This is SlaveSignalEvent.
This is MasterSignalEvent.

SPI DMA transfer all data matched!
