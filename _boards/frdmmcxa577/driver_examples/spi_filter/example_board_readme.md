Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
- Personal Computer

Board settings
============
EVB rework:
SJ13 2-3, SJ14 2-3, SJ26 2-3, SJ15 2-3, SJ71 1-2-3, SJ80  1-3, SJ82 1-3, SJ68 1-2,
SJ69 1-2, SJ16 2-3, SJ25 1-2(Default), SJ70 1-2, SJ81 1-2-3, SJ83 1-2-3, SJ17-20 2-3,
SJ21-24 1-2(Default), SJ64 1-2, SJ65-67 1-3, SJ72-75 1-3, SJ76-79 1-2-3.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI Filter example start
Get Vendor ID 0xEF
==== Opcode filter test ====
Configured opcode 0x95
Command 0x95 is not blocked. Opcode status 0x00
Configured opcode 0x00
Command 0x95 is blocked
==== Memory region filter test ====
Set filter region: 0x00010000 - 0x00020000
Touch memory address: 0x00010000
Erase data - successfully.
Program data - successfully.
Touch memory address: 0x00020000
Program data - Blocked by SPI Filter.
SPI Filter example finished
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
