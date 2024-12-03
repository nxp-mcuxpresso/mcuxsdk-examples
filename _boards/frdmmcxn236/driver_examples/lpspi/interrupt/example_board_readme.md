Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN236 board
- Personal Computer

Board settings
============

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J5-6                 SIN       J2-10
SIN        J1-4                 SOUT      J2-8
SCK        J5-5                 SCK       J2-12
PCS0       J1-2                 PCS0      J2-6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal on PC for MCU-Link serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
