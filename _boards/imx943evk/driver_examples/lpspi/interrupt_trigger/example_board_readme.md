Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-EVK board
- Personal Computer

Board settings
============

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER(LPSPI3)      connect to    SLAVE(LPSPI8)
Pin Name   Board Location     Pin Name    Board Location
SOUT       J47-8                SIN       J43-10
SIN        J47-10               SOUT      J43-8
SCK        J47-12               SCK       J43-12
PCS0       J47-6                PCS0      J43-6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
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


Please make sure you make the correct line connection. Basically, the connection is:

LPSPI_master -- LPSPI_slave

   CLK      --    CLK

   PCS      --    PCS

   SOUT     --    SIN

   SIN      --    SOUT

LPSPI transfer all data matched!

End of example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
