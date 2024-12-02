Hardware requirements
=====================
- Mini USB cable
- Two FRDM-K32L3A6 boards
- Personal Computer

Board settings
============
SPI two board:
Transfer data from one board instance to another board's instance.
To make lpspi example work(M4), connections needed to be as follows:
SPI0 pins are connected with SPI0 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE0(SPI0)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J2 pin 8            SIN       J2 pin 10
SIN         J2 pin 10           SOUT      J2 pin 8
SCK         J2 pin 12           SCK       J2 pin 12
PCS2        J2 pin 6            PCS2      J2 pin 6
GND         J2 pin 14           GND       J2 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To make lpspi example work(M0), connections needed to be as follows:
SPI3 pins are connected with SPI3 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE3(SPI3)     CONNECTS TO         INSTANCE3(SPI3)
Pin Name   Board Location     Pin Name  Board Location
SOUT        J1 pin 2            SIN       J1 pin 6
SIN         J1 pin 6            SOUT      J1 pin 2
SCK         J3 pin 5            SCK       J3 pin 5
PCS1        J3 pin 9            PCS1      J3 pin 9
GND         J2 pin 14           GND       J2 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


