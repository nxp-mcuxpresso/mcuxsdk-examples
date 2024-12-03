# lpspi_edma_transfer

## Overview
The lpspi_edma example shows how to use LPSPI driver in edma way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in edma . (LPSPI Slave using edma to receive/send the data)


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.


LPSPI edma example start.
This example use one lpspi instance as master and another as slave on one board.
Both master and slave use edma way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT

This is LPSPI slave edma transfer completed callback.
It's a successful transfer.

LPSPI transfer all data matched!

End of example.
