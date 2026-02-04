  Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC162 Board
- Personal Computer

Board settings
Positive input channel J1-13(P2_12).
OPAMP output pin: J2-1(P2_16).
Negative input channel J1_15(P2_13) connects to GND.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for the description of how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One-stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
OPAMP BASIC EXAMPLE!


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OPAMP output voltage, ranging 0-3.3v, is about twice the input voltage. 