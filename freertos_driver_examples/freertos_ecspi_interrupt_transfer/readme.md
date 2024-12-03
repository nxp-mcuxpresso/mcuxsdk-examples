# freertos_ecspi_interrupt_transfer

## Overview
The freertos_ecspi example shows how to use ECSPI driver in FreeRTOS:

In this example , one ecspi instance is used as ECSPI master with blocking and another ecspi instance is used as ECSPI slave.

1. ECSPI master sends/receives data using task blocking calls to/from ECSPI slave. (ECSPI Slave uses interrupt to receive/send
the data)

The example supports board to board connection.

With board to board connection, one ECSPI instance on one board is used as ECSPI master and the ECSPI instance on other board is used as ECSPI slave. Tasks are created to run on each board to handle ECSPI communication.
    File freertos_ecspi.c should have following definitions:
    #define EXAMPLE_CONNECT_ECSPI BOARD_TO_BOARD
    For board used as ECSPI master:
        #define ECSPI_MASTER_SLAVE isMASTER
    For board used as ECSPI slave:
        #define ECSPI_MASTER_SLAVE isSLAVE

## Running the demo
When the example runs successfully, you can see the similar information on the terminal as below.

For master:

FreeRTOS ECSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
ECSPI_master -- ECSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 
ECSPI master transfer completed successfully. 

ECSPI transfer all data matched!

For slave:

FreeRTOS ECSPI example start.
This example use two boards to connect with one as master and anohter as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
ECSPI_master -- ECSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 
ECSPI slave transfer completed successfully.

ECSPI transfer all data matched!

## Supported Boards
