Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA156 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
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

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRDC example start
Set the MBC selected memory block not accessible
Resolve access error
The MBC selected block is accessible now
TRDC example Success
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~