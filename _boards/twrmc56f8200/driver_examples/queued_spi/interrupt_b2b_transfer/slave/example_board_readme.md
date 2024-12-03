Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8200 board
- Personal Computer

Board hardware common setting
=============================
1. HW reworkd(workaround):
   > Connect J18(the mini USB port) pin4 and pin5.
     Note: This is due to HW design error, correct setting should be: J18 pin4 left empty and pin5 connected GND.
2. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
3. UART setting: 
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
4. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
  Master/Slave board connection as below:
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Master Board                             Slave Board
  INSTANCE0(QSPI0)     CONNECTS TO         INSTANCE1(QSPI0)
  Pin Name   Board Location      Pin Name  Board Location
  SS0        J502 pin 16         SS0        J502 pin 16
  MOSI       J502 pin 22         MOSI       J502 pin 22
  MISO       J502 pin 18         MISO       J502 pin 18
  SCK        J502 pin 20         SCK        J502 pin 20
  GND        J502 pin 6          GND        J502 pin 6
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QUEUEDSPI board to board interrupt example.

 Slave example is running...
This is QUEUEDSPI slave transfer completed callback. 
It's a successful transfer. 

QUEUEDSPI transfer all data matched! 

 Slave receive:
      1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
     11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
     21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
     31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

 Slave example is running...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
