Hardware requirements
===================
- Mini/micro USB cable
- LPCXpresso55S06 board
- Personal Computer

Board settings
============
(pmux)J11-2(PIO0_0) connect to voltage source1.
(nmux)J11-1(PIO0_9) connect to voltage source2.

Prepare the Demo
===============
1.  Connect a macro USB cable between the PC host and the OpenSDA USB port on the board.
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

CMP interrupt driver example.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Then change CMP analog input, and the LED GREEN will show the output of the comparator.
    If the voltage of pmux channel is higher than the voltage of nmux channel, the GREEN LED on the board will be turned on.
    If the voltage of pmux channel is lower than the voltage of nmux channel, the GREEN LED on the board will be turned off.
