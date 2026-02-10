Hardware requirements
=====================
- Type-C USB cable
- RDM-MCXW72 Board
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
Jumper setting:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(SPI1)                connect to      SLAVE(SPI1)
Pin Name   Board Location                   Pin Name    Board Location
SOUT       J2 pin 7                         SIN         J2 pin 6
SIN        J2 pin 6                         SOUT        J2 pin 7
SCK        J2 pin 5                         SCK         J2 pin 5
PCS0       J2 pin 8                         PCS0        J2 pin 8
GND        J2 pin 4                         GND         J2 pin 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Other jumpers keep default configuration.

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



