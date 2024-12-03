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

ELS hash example

============================
SHA224 one shot:pass
SHA256 one shot:pass
SHA256 streaming example:pass
SHA256 long message example:pass
SHA384 one shot:pass
SHA512 one shot:pass

============================
RESULT: All 6 test PASS!!
ELS example END

