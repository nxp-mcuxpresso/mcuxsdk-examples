Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
==============
Populate R126, R128, R129, R130, R131, R133.
Remove R153, R154.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW701 to power on the board
2.  Connect a USB cable between the host PC and the J1701 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
QSPI example started!
Erase finished!
Program data finished!
Program through QSPI EDMA succeed!
~~~~~~~~~~~~~~~~~~~~~
