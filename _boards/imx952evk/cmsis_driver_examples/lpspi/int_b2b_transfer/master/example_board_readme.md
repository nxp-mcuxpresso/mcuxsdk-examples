Hardware requirements
=====================
- Micro USB cable
- two IMX952-EVK boards
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
set SW1 into 0000 on IMX952-EVK

Transfer data from one board instance to another board's instance.
SPI7 pins are connected with SPI7 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI7)     CONNECTS TO         INSTANCE0(SPI7)
Pin Name   Board Location       Pin Name  Board Location
SPI_SCK    IMX952LPD5EVK-19   JP1   pin 5	  SPI7_SCK  IMX952LPD5EVK-19    JP1 pin 5
SPI_SIN    IMX952LPD5EVK-19   JP1   pin 3          SPI7_SOUT IMX952LPD5EVK-19    JP1 pin 4
SPI_SOUT   IMX952LPD5EVK-19   JP1   pin 4          SPI7_SIN  IMX952LPD5EVK-19    JP1 pin 3
SPI_PCS0   IMX952LPD5EVK-19   JP1   pin 2          SPI7_PCS0 IMX952LPD5EVK-19    JP1 pin 2
GND                             GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either cold boot your board or launch the debugger in your IDE to begin running the example.


