Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer

Board settings
============
Weld 0Ω resistor to R346,R350,R356,R362.

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J17-4                SIN       J17-5
SIN        J17-5                SOUT      J17-4
SCK        J17-6                SCK       J17-6
PCS0       J17-3                PCS0      J17-3
GND        J17-7                GND       J17-7
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


