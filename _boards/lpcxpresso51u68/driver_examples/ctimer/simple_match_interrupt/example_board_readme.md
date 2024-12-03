Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso51U68 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J6) on the board
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
The log below shows example output of the CTimer simple match demo using interrupts in the terminal window, you could also monitor the output on J2-5.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CTimer match example to toggle the output.
This example uses interrupt to change the match period.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
