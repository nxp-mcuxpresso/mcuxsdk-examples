Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- MCUXpresso10.1.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

Board settings
==============
- ADC CH0 input signal J6-12(PIO0-7).

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J4) on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Monitor the information on the debug console.

## Expected Result
------------------------
Press any key and print user channel's result in serial terminal.
~~~~~~~~~~~~~~~~~~~~~
