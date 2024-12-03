Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860max board
- Personal Computer

Board settings
============
- ADC CH0 input signal J6-12(PIO0-7).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J4) on the board
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press reset button to begin running the demo.

Running the demo
================
Monitor the information on the debug console.

## Expected Result
Press any key and print user channel's result in serial terminal.
~~~~~~~~~~~~~~~~~~~~~
