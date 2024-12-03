Hardware requirements
=====================
- J-Link ARM
- FRDM-K32L3A6 board
- Mini/micro USB cable
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================

When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Press anykey to start the example...

EWM example
Press SW2 to expire EWM
EWM interrupt is occurred
Press SW2 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~