Hardware requirements
=====================
- Type-C USB cable
- TBD board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRDC example start
Set the MRC selected memory region not accessible
Violent access at address:  0x25000000
The MRC selected region is accessible now
Set the MBC selected memory block not accessible
Violent access at address: 0x43800000
The MBC selected block is accessible now
TRDC example Success
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
