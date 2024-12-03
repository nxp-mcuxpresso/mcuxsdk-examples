Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J1) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Utick example start...
UTICK delay 1 second...
UTICK delay 1 second...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~