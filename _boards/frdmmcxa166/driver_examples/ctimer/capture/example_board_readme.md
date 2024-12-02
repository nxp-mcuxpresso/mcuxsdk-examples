Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA166 board
- Personal Computer

Board settings
============
Short J2-18(P1_8) and J2-20(P1_9).

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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
The CTimer captured rising edge timestamp is print in the terminal periodically.
