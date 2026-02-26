Hardware requirements
=====================
- Micro USB cable
- one FRDM-IMX95 board and MCIMX93EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Transfer data from one board instance to another board's instance.
SPI3 pins are connected with SPI3 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI3)     CONNECTS TO         INSTANCE0(SPI3)
Pin Name   Board 	     Location       	  Pin Name  Board         Location
SPI3_SCK   FRDMIMX95    J19 pin 23	  SPI3_SCK  FRDMIMX95    J19 pin 23
SPI3_SIN   FRDMIMX95    J19 pin 21          SPI3_SOUT FRDMIMX95    J19 pin 19
SPI3_SOUT  FRDMIMX95    J19 pin 19          SPI3_SIN  FRDMIMX95    J19 pin 21
SPI3_PCS0  FRDMIMX95    J19 pin 24          SPI3_PCS0 FRDMIMX95    J19 pin 24
GND                                               GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either cold boot your board or launch the debugger in your IDE to begin running the example.


