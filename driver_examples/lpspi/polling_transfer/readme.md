# lpspi_polling_transfer

## Overview
The lpspi_polling example shows how to use LPSPI driver in polling way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in polling . (LPSPI Slave using interrupt to receive/send the data)

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI polling example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses polling way and slave uses interrupt way.
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
