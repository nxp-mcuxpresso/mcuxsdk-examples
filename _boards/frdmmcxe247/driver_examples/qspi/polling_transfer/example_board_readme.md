Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer

Board settings
==============
Populate the R120, R122, R124, R126, R129, R130 on board.
Remove the U16, U17 on board.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
QSPI example started!
Erase finished!
Program data finished!
Program through QSPI polling succeed!
~~~~~~~~~~~~~~~~~~~~~
