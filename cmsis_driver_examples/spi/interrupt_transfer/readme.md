# cmsis_spi_interrupt_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_spi_interrupt_transfer example shows how to use CMSIS SPI driver in interrupt way:

In this example , one spi instance used as master and another spi instance used as slave in the same board.

1. SPI master send/received data to/from SPI slave in interrupt transfer . (SPI Slave using interrupt to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

SPI CMSIS interrupt transfer start.
This example use one spi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is: 
SPI_master -- SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS  
   MOSI     --    MOSI 
   MISO     --    MISO 
This is SPI_SlaveSignalEvent_t
This is SPI_MasterSignalEvent_t
 
SPI transfer all data matched! 


