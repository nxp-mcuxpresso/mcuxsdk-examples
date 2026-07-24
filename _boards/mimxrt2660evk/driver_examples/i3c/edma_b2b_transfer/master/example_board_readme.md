Hardware requirements
=====================
- Type-C USB cable
- Two MIMXRT2660-EVK boards
- Personal Computer

Board settings
==============
Weld 1.5kΩ to R778
I3C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of I3C interface, I3C pins of MASTER_BOARD are connected with
    I3C pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name   Board Location      Pin Name   Board Location
I3C_SCL    J34-20 (PIO2_25)   I3C_SCL    J34-20 (PIO2_25)
I3C_SDA    J34-18 (PIO2_24)   I3C_SDA    J34-18 (PIO2_24)
GND                             GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
