Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
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

ERM error recording example.

Flipped bit was corrected.

ERM error recording address is 0x28001f00.

ERM error recording example finished successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
