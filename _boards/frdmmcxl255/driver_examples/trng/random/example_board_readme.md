Hardware requirements
=====================
- USB-C cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
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

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Generate 10 random numbers:
Random[0] = 0xED89EEA6
Random[1] = 0xE6BFE1AD
Random[2] = 0x8F7B8048
Random[3] = 0x28E7548F
Random[4] = 0x93CC02B7
Random[5] = 0xCF178D2F
Random[6] = 0xD5947CDD
Random[7] = 0x255E1955
Random[8] = 0xFBA3BF91
Random[9] = 0xC26C7ACE

 Press any key to continue...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~