Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============

LPSPI:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT        J11 pin 8           SIN       J11 pin 10
SIN         J11 pin 10          SOUT      J11 pin 8
SCK         J11 pin 12          SCK       J11 pin 12
PCS0        J11 pin 6           PCS0      J11 pin 6 
GND         J11 pin 14          GND       J11 pin 14
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
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


