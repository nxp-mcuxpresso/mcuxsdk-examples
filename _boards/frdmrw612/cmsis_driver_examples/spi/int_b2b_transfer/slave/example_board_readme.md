Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
============
Connect SPI master on board to SPI slave on other board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI1)           Slave_board(SPI1)
Pin Name   Board Location     Pin Name   Board Location
MISO       J1 pin 4           MISO       J1 pin 4
MOSI       J1 pin 2           MOSI       J1 pin 2
SCK        J2 pin 12          SCK        J2 pin 12
PCS0       J2 pin 6           PCS0       J2 pin 6
GND        J3 pin 14          GND        J3 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI CMSIS driver board to board interrupt example.

 Slave example is running...


 Slave receive:
      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
     10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
     20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
     30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
