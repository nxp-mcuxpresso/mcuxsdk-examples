Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
Jumper setting:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(SPI1)                connect to      SLAVE(SPI1)
Pin Name   Board Location                   Pin Name    Board Location
SOUT       J19 pin 5                        SIN         J19 pin 4
SIN        J19 pin 4                        SOUT        J19 pin 5
SCK        J19 pin 6                        SCK         J19 pin 6
PCS0       J19 pin 3                        PCS0        J19 pin 3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Other jumpers keep default configuration.

Prepare the Demo
================
1. Connect a Type-C USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.



