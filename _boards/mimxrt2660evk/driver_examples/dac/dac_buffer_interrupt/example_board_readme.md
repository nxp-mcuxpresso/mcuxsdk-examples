Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
DAC output: testpoint TP29 (ball L5, with C142 100pF filter to GND).
A multimeter or oscilloscope probe can be used to measure at TP29.

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
5.  A multimeter may be used to measure the DAC output voltage at TP29.

Running the demo
===============
When the demo runs successfully, the log would be seen on the debug console terminal like:

DAC buffer interrupt Example.
Press any key to trigger the DAC...
DAC next output: 200
DAC next output: 300
DAC next output: 400
DAC next output: 500
DAC next output: 600
DAC next output: 700
DAC next output: 800
DAC next output: 900

Then user can measure the DAC output pin to check responding voltage.

