# lpspi_interrupt_trigger

## Overview
The lpspi_functional_interrupt_trigger example shows how to use LPSPI driver in interrupt way with trigger signal:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in interrupti when there is a trigger signal . (LPSPI Slave using interrupt to receive/send the data)


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

LPSPI functional interrupt example start.
This example use one lpspi instance as master and another as slave on one board.
Master uses interrupt way and slave uses interrupt way.
Note that some LPSPI instances interrupt is in INTMUX ,you should set the intmux when you porting this example accordingly
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK      --    CLK
   PCS      --    PCS
   SOUT     --    SIN
   SIN      --    SOUT

LPSPI transfer all data matched!

End of example.

