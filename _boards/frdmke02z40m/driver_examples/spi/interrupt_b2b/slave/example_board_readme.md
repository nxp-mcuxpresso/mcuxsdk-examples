Hardware requirements
===================
- Mini USB cable
- Two FRDM-KE02Z40M boards
- Personal Computer

Board settings
============
SPI board to board:
Transfers data through SPI interface. SPI0 pins of the master board are
connected to the SPI0 pins of the slave board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER BOARD           CONNECTS TO         SLAVE BOARD
Pin Name   Board Location     Pin Name  Board Location
MISO       J9 pin 5           MISO      J9 pin 5
MOSI       J9 pin 3           MOSI      J9 pin 3
SCK        J9 pin 1           SCK       J9 pin 1
PCS0       J9 pin 7           PCS0      J9 pin 7
GND        J9 pin 14          GND       J9 pin 14
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

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI board to board interrupt slave example started!

SPI transfer finished!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
