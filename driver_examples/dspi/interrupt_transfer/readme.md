# dspi_interrupt_transfer

## Overview
The dspi_interrupt_transfer example shows how to use DSPI driver in interrupt way:

In this example , one dspi instance used as DSPI master and another dspi instance used as DSPI slave in the same board.
This example uses the transactional API in DSPI driver.

1. DSPI master send/received data to/from DSPI slave in interrupt . (DSPI Slave using interrupt to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

DSPI interrupt example start.
This example use one dspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
DSPI_master -- DSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT
This is DSPI slave call back .

DSPI transfer all data matched!
