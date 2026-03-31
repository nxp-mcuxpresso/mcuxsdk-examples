
Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
==============

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE1(LPSPI_1)     CONNECTS TO         INSTANCE1(LPSPI_1)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J6 pin 6           SIN       J6 pin 5
SIN         J6 pin 5           SOUT      J6 pin 6
SCK         J6 pin 4           SCK       J6 pin 4
PCS0        J6 pin 3           PCS0      J6 pin 3 
GND         J6 pin 8           GND       J6 pin 8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2. Open a serial terminal on PC for MCU-Link serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

