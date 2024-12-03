Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
SHELL>> led 2 off
SHELL>> led 3 on
SHELL>> led 3 off
SHELL>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
