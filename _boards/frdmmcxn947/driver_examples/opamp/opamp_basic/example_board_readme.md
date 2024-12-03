  Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
Positive input channel J4-1(P4_12).
OPAMP output pin: J8-20(P4_15).
Negative input channel J4_3(OPAMP0_INN) connects to GND.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the PC host and the CMSIS DAP USB port (J17) on the board
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