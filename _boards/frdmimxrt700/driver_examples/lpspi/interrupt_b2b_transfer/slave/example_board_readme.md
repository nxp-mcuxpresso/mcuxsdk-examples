Hardware requirements
=====================
- Type-C USB cable
- Two FRDM-IMXRT700 boards
- Personal Computer

Board settings
============
1. Make sure JP12 is installed
2. SPI two boards:
Transfer data from one board instance to another board's instance.
SPI16 pins are connected with SPI16 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI16)     CONNECTS TO         INSTANCE0(SPI16)
Pin Name   Board Location           Pin Name  Board Location
SOUT        J22 pin 6                   SIN       J22 pin 5
SIN         J22 pin 5                   SOUT      J22 pin 6
SCK         J22 pin 4                   SCK       J22 pin 4
PCS0        J22 pin 3                   PCS0      J22 pin 3
GND         J22 pin 8                   GND       J22 pin 8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port on the board.
2.  Open a serial terminal on PC for MCU-Link serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


