Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW10 to power on the board.
2.  Connect a micro USB cable between the host PC and the J17 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
==FlexCAN loopback functional example -- Start.==

Send message from MB0 to MB1
tx word0 = 0x0
tx word1 = 0x1
tx word2 = 0x2
tx word3 = 0x3
tx word4 = 0x4
tx word5 = 0x5
tx word6 = 0x6
tx word7 = 0x7

Received message from MB1
rx word0 = 0x0
rx word1 = 0x1
rx word2 = 0x2
rx word3 = 0x3
rx word4 = 0x4
rx word5 = 0x5
rx word6 = 0x6
rx word7 = 0x7

==FlexCAN loopback functional example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~

