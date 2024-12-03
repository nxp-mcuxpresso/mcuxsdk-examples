Hardware requirements
=====================
- Mini/micro USB cable
- EVB-MIMXRT1060 board
- Personal Computer

Board settings
============
Make sure nand flash is avaliable on board.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like below, after shell is avaliable, please start with command 
"help".
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nand flash management demo.

Make file system..............

Create directory......

SHELL (build: Feb 11 2018)

 * Copyright 2021 NXP

SHELL>>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
