Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer

Board settings
==============
The example requires connecting between LPSPI0 pins and LPSPI2 pins
The connection should be set as following:
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name  Board Location
SOUT        J6 pin 6            SIN       J7 pin 5
SIN         J6 pin 5            SOUT      J7 pin 3
SCK         J6 pin 4            SCK       J7 pin 7
PCS2        J6 pin 3            PCS1      J7 pin 1

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~
