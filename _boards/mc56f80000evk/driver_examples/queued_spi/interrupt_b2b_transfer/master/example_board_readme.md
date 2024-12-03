Hardware requirements
=====================
- Micro USB cable
- MC56F80000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J6 pin1 and pin2
   > Connect J5 pin1 and pin2
2. Reset pin setting:
   > Connect J11 pin2 and pin3
3. Debugger setting:
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8

Board settings
==============
  Master/Slave board connection as below:
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Master Board                             Slave Board
  INSTANCE0(QSPI0)     CONNECTS TO         INSTANCE1(QSPI0)
  Pin Name   Board Location      Pin Name  Board Location
  SS0        J1 pin 3            SS0       J1 pin 3
  MOSI       J2 pin 8            MOSI      J2 pin 8
  MISO       J2 pin 10           MISO      J2 pin 10
  SCK        J2 pin 12           SCK       J2 pin 12
  GND        J2 pin 14           GND       J2 pin 14
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Note: Don't use GC7 as SS since GC7 is used for onboard MX25

Prepare the Demo
================
1.  Connect USB cable between the host PC and the J12 USB port on the target board. It setups Multilink and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with Multilink debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QUEUEDSPI board to board interrupt example.
This example uses one board as master and another as slave.
Master and slave use interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
QSPI_master -- QSPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   SOUT     --    SIN  
   SIN      --    SOUT 
   GND      --    GND 

 Master transmit:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40
 
QUEUEDSPI transfer all data matched! 

 Master received:

  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Press any key to run again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
