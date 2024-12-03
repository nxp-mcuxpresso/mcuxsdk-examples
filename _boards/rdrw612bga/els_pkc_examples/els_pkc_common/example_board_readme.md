Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the terminal will display similar information like the following:

ELS PKC common example

============================
ELS get info:pass 
RNG PRNG random:pass 
Flow protection:pass 
Memory data invariant compare:pass 
Memory data invariant copy:pass 
Key component operations:pass 
PKC power down wake-up init:pass 

============================
RESULT: All 7 test PASS!!
ELS example END 

