Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
The example requires connecting between LPSPI0 pins and LPSPI1 pins
The connection should be set as following:
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name  Board Location
SOUT       J3 pin 7            SIN       J2 pin 10
SIN        J3 pin 9            SOUT      J2 pin 8
SCK        J3 pin 11           SCK       J2 pin 12
PCS3       J3 pin 5            PCS3      J2 pin 6

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

Running the demo
================
${ANCHOR}
