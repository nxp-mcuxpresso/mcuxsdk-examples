Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54018 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.

Running the demo
================
Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

The following lines are printed to the serial terminal when the demo program is executed.
 
 GPIO Driver example

 The LED is taking turns to shine.

 Standard port read: ffbff7ff

 Masked port read: bfbff7ff

 Port state: bfbff7ff
 
Then press the USER, the LD21&&LE22 will start to shine.

Press corresponding switches to generate events
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
