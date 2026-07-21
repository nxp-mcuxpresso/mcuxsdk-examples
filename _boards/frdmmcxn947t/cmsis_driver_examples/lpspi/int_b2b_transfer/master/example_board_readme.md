Hardware requirements
=====================
- Mini USB cable
- Two FRDM-MCXN947T boards
- Personal Computer

Board settings

LPSPI:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MASTER           connect to           SLAVE
Pin Name   Board Location     Pin Name    Board Location
SOUT       J2 Pin 8           SIN         J2 Pin 10
SIN        J2 Pin 10          SOUT        J2 Pin 8
SCK        J2 Pin 12          SCK         J2 Pin 12
PCS0       J2 Pin 6           PCS0        J2 Pin 6
GND        J2 pin 14          GND         J2 pin 14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the MCU-Link USB port on the board.
2.  Open a serial terminal on PC for MCU-Link serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.


