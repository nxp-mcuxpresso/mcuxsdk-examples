Hardware requirements
=====================
- Two Type-C USB cables
- Two FRDM-MCXN947 boards
- Personal Computer

Board settings
============
I3C board to board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD through the I3C interface, I3C1 pins of
    MASTER_BOARD (SCL: P1_17, SDA: P1_16) are connected with I3C1 pins of SLAVE_BOARD.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD(FRDM-MCXN947)   CONNECTS TO   SLAVE_BOARD(FRDM-MCXN947)
Pin Name   Board Location               Pin Name   Board Location
P1_16      J5-3                         P1_16      J5-3
P1_17      J5-4                         P1_17      J5-4
GND        J5-8                         GND        J5-8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  Reset the SLAVE_BOARD first, then reset the MASTER_BOARD.
