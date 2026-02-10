Hardware requirements
=====================
- USB-C cable
- Two FRDM-MCXE247 boards
- Personal Computer

Board settings
============
SPI two boards:
Transfer data from one board instance to another board's instance.
SPI0 pins are connected with SPI0 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE0(SPI0)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J6 pin 6            SIN       J6 pin 5
SIN         J6 pin 5            SOUT      J6 pin 6
SCK         J6 pin 4            SCK       J6 pin 4
PCS2        J6 pin 3            PCS2      J6 pin 3
GND         J6 pin 8            GND       J6 pin 8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the MCU-LINK USB port on the board.
2.  Open a serial terminal on PC for MCU-LINK serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


