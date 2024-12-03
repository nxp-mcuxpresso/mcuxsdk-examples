# cmsis_lpspi_interrupt_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_lpspi_interrupt_transfer example shows how to use CMSIS LPSPI driver in interrupt way:

In this example , one SPI instance used as master and another SPI instance used as slave in the same board.

LPSPI master send/received data to/from slave in edma . (LPSPI Slave using edma to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI CMSIS interrupt example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses interrupt way and slave uses interrupt way.
Note that some LPSPI instances interrupt is in INTMUX ,you should set the intmux when you porting this example accordingly 
Please make sure you make the correct line connection. Basically, the connection is: 
LPSPI_master -- LPSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   SOUT     --    SIN  
   SIN      --    SOUT 
This is LPSPI_SlaveSignalEvent_t
 
LPSPI transfer all data matched! 

