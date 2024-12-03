# cmsis_lpspi_edma_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_lpspi_edma_transfer example shows how to use CMSIS LPSPI driver in edma way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave in the same board.

LPSPI master send/received data to/from LPSPI slave in edma . (LPSPI Slave using edma to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI CMSIS edma transfer example start.
This example use one LPSPI instance as master and another as slave on one board.
Master use edma way , slave uses interrupt.
Please make sure you make the correct line connection. Basically, the connection is: 
LPSPI_master -- LPSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS  
   SOUT     --    SIN  
   SIN      --    SOUT 
This is SlaveSignalEvent.
 
LPSPI transfer all data matched! 

