Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
================
The log below shows the output of the coremark demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Trigger Coremark...

2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 103821119
Total time (secs): 1038211.190000
Iterations/Sec   : 0.000096
Iterations       : 100
Compiler version : ARM GCC
Compiler flags   : UNKNOWN CC FLAGS
Memory location  : STATIC
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 0.000096 / ARM GCC UNKNOWN CC FLAGS / STATIC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
