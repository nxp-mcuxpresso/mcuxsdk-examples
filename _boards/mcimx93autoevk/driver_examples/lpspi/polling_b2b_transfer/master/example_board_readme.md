Hardware requirements
=====================
- USB Type-C cable
- two MCIMX93AUTO-EVK boards
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Transfer data from one board instance to another board's instance.
SPI3 pins are connected with SPI3 pins of another board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI3)     CONNECTS TO         INSTANCE0(SPI3)
Pin Name   Board Location       Pin Name  Board Location
SPI3_SCK    J16  pin  23        SPI3_SCK  J16  pin  23
SPI3_SIN    J16  pin  21        SPI3_SOUT J16  pin  19
SPI3_SOUT   J16  pin  19        SPI3_SIN  J16  pin  21
SPI3_PCS0   J16  pin  24        SPI3_PCS0 J16  pin  24
GND                             GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW2 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J26 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either cold boot your board or launch the debugger in your IDE to begin running the example.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI board to board polling example.
This example use one board as master and another as slave.
Master uses polling way and slave uses interrupt way. 
Please make sure you make the correct line connection. Basically, the connection is: 
LPSPI_master --  LPSPI_slave   
   CLK       --    CLK  
   PCS       --    PCS 
   SOUT      --    SIN  
   SIN       --    SOUT 
   GND       --    GND 

 Master transmit:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40
 
LPSPI transfer all data matched! 

 Master received:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
