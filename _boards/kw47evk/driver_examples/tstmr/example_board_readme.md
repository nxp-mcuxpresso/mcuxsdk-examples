Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Timestamp1 = 0627

 Test the delay function, delay for 1 second

 Start time = 01e69

 End time = 0f689e
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
