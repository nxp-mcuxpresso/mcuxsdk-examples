Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J33.
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
Copyright  2024  NXP

SHELL>> help

"help": List all the registered commands

"exit": Exit program

"led arg1 arg2":
 Usage:
    arg1: 1|2|3|4...            Led index
    arg2: on|off                Led status

SHELL>> led 1 on
SHELL>> led 1 off
SHELL>> led 2 on
LED index is wrong
SHELL>> 
~~~~~~~~~~~~~~~~~~~~~
