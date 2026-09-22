Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
============
- OPAMP0_INP input signal J1-13(P1_11).
- OPAMP0_INN input signal J1-15(P1_12).

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OPAMP Interrupt Example
OPAMP initialized and enabled.
ADC Full Range: 4096
Please press any key to get user channel's ADC value.
ADC value: 3115
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
