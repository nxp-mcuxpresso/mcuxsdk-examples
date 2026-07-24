Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
SPI two boards:
Transfer data from one board instance to another board's instance.
LPSPI1 pins are connected with LPSPI1 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(LPSPI1)     CONNECTS TO    INSTANCE(LPSPI1)
Pin Name   Board Location      Pin Name   Board Location
SOUT       J94-28 (PIO3_15)    SIN        J94-9  (PIO3_16)
SIN        J94-9  (PIO3_16)    SOUT       J94-28 (PIO3_15)
SCK        J94-27 (PIO3_14)    SCK        J94-27 (PIO3_14)
PCS0       J94-26 (PIO3_13)    PCS0       J94-26 (PIO3_13)
GND                             GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

