Hardware requirements
===================
- Mini/micro USB cable
- LPCxpresso55s69 board
- Personal Computer

Board settings
============
Connect a DC voltage source to P19_6 (PIO0_0/CMP IN1)

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

CMP polling driver example

Then change CMP analog input, and the LED GREEN will show the output of the comparator.
    - Let CMP IN1 voltage be higher than half of the VDDA, LED GREEN on.
    - Let CMP IN1 voltage be lower than half of the VDDA, LED GREEN off.
