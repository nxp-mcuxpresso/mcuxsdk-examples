Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXC041 board
- Personal Computer

Board settings
============
SPI one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of SPI interface, SPI0 pins of MASTER_BOARD are connected with
    SPI1 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
           MASTER_BOARD    CONNECTS TO   SLAVE_BOARD
Pin Name   Board Location                Board Location
MOSI       J2-8                          J2-8
MISO       J2-10                         J2-10
SS         J2-6                          J2-6
CLK        J2-12                         J2-12
GND        J2-14                         J2-14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a Type-C USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 9600 baud rate
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
Master Start...

Succeed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
