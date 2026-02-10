Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVKB board
- Personal Computer

Board settings
============
Remove 0Ω resistors R200,R404,R406.

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J10-8                SIN       J10-10
SIN        J10-10               SOUT      J10-8
SCK        J10-12               SCK       J10-12
PCS0       J10-6                PCS0      J10-6
GND        J10-14               GND       J10-14
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


