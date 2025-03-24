Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
Use the default jumper settings.

Output pin		Board location
CTIMER2_MAT0        J21_1 (PWM)
CTIMER2_MAT1        J21_2 (INT)

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
The log below shows example output of the CTimer simple match demo using interrupts in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CTimer match example to toggle the output. 
This example uses interrupt to change the match period.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Probe an oscilloscope to the output pins described in board settings, you will see The two output signals alternating frequently.
You can see the red led will blinking if you short the ctimer output pins and led pin J36-7.
