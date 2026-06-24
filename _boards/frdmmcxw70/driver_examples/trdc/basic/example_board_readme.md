Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J28.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  The demo will measure the frequency of some on-chip clock source.
    Use the Oscilloscope to probe SFA0_CLK Pin then you will monitor the signal to be measured by SFA.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRDC example start
Set the MBC selected memory block not accessible
Violent access at address: 0x20001000
The MBC selected block is accessible now
TRDC example Success
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
