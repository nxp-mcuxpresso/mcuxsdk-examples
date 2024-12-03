Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the CMSIS DAP port(J4 on the board).
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.

Running the demo
================
1.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.

IAP Flash example

Writing flash sector 30

Erasing flash sector 30

Erasing page 1 in flash sector 30

Flash signature value of page 30

XXXXXXXXXXXXXXXXXXXXX

End of IAP Flash Example 
