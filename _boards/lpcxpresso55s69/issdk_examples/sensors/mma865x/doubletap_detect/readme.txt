Hardware requirements
=======================
- Mini/micro USB cable
- LPC55S69-EVK board, Rev 2
- Personal Computer

Demo Modes:
===========
DOUBLE-TAP DETECTION: The on-board MMA8652 example application demonstrating MMA8652 configurations for enabling double-tap detection.

Demo Overview
==============
- The on-board MMA8652 example application demonstrating double-tap detection feature. The example demonstrates configuration of all registers
  threshold values required to put the sensor into double-tap detection mode.

Prepare the Demos
===================
1.  Connect USB cable between the host PC and the Debug Link USB port on the LPC55S69-EVK board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the LPC55S69-EVK board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demos
===================
When this demo runs successfully, apply double-tap (you can use a pen or a pencil to apply double-tap) on the board and application will display
double-tap event detection.

Following is a snapshot of demo application output on serial terminal:

 ISSDK MMA865x sensor driver example for Double-Tap detection.
 Successfully Initialized Sensor
 Successfully Applied MMA865x Sensor Configuration

 MMA865x is now active and detecting double-tap...

 Double-Tap Detected !!!
 Double-Tap Detected !!!
