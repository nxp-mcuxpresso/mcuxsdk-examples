Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============
IMPORTANT: Connect pin 2-3 of JP1 and JP56 (sets VCC_1V9 for MCU and flash)
Connect pin 1-2 of JP41,JP42,JP43,JP44,JP45,JP64 and JP65.
Close JP66,JP67,JP68 and JP69.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SHELL (build: Feb 28 2018)
 * Copyright 2021 NXP
LFS>>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~