Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer
- Oscilloscope

Board settings
============

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the EVK board J14.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  The demo will measure the frequency of some on-chip clock source.
    Use the Oscilloscope to probe SFA0_CLK Pin then you will monitor the signal to be measured by SFA.

Running the demo
================
The log below shows the output of the SFA polling demo in the terminal window(There may be some deviation in the measurement results):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SFA example -- Measure the frequency of clock under test in polling mode
Start to measure Fast Clock.
The actual frequency of Fast Clock is 96963392 Hz.
Start to measure Slow clock.
The actual frequency of the Slow Clock is 33059 Hz.

SFA Measurement Finished.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
