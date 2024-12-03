Toolchain supported
===================
- IAR embedded Workbench 8.22.1
- Keil MDK 5.24a
- MCUXpresso10.2.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso804 board
- CapTouch Shield board
- Personal Computer

Board settings
==============
Remove JP3, JP21, JP22, JP23, JP25 and C13 on LPCXpresso804 board.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When touch the electrode plate, the corresponding LED would turn on.
