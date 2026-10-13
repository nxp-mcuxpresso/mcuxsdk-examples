Hardware requirements
=====================
- USB Type-C cable
- FRDM-RW610FML board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a USB Type-C cable between the PC host and the MCU-Link USB port (J15) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Usart polling example
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
