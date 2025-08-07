Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
If you want to run this example on the MCXW235 RDM module, please regenerate the peripherals.c .h of
MCXW235 using the TEE tool. LITTLEFS_START_ADDR can be set to 0x3e000 because the flash size of
MCXW235 is 512KB.

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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Copyright  2024  NXP

LFS>>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
