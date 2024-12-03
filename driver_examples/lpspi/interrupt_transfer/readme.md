# lpspi_interrupt_transfer

## Overview
The lpspi_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave.

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI interrupt example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses interrupt way and slave uses interrupt way.
Note that some LPSPI instances interrupt is in INTMUX ,you should set the intmux when you porting this example accordingly
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT

This is LPSPI slave transfer completed callback.
It's a successful transfer.

LPSPI transfer all data matched!

End of example.
