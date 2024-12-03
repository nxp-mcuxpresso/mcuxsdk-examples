Hardware requirements
=======================
- Mini/micro USB cable
- LPC55S69-EVK board, Rev 2
- Personal Computer

Demo Mode:
===========
AUTO_WAKE_SLEEP: The on-board MMA8652 example application demonstrating MMA8652 configurations for enabling Auto Wake Sleep feature.

Demo Overview
==============
- The on-board MMA8652 example application demonstrating Auto Wake Sleep feature. The example demonstrates configuration of all registers
  threshold values required to put the sensor into motion detection and auto-sleep mode.

Prepare the Demos
===================
1.  Connect USB cable between the host PC and the Debug Link USB port on the LPC55S69-EVK board.
2.  Open a serial terminal (TertaTerm, Putty or RealTerm) with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the LPC55S69-EVK board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demos
===================
When this demo runs successfully, perform following steps to view Auto-Wake-Sleep demonstration.

- Move the board (or lift the board up), on-board MMA8652 will detect motion. Status RGB LED will glow GREEN LED to show sensor in wake mode.
- Do not move board, on-board MMA8652 will detect inactivity and on expiry of ASLP count (~6 secs) it will go to
  sleep-mode automatically. Status RGB LED will glow RED LED to show sensor in sleep mode.
- Move the board again (or lift the board up), on-board MMA8652 will detect motion. Status RGB LED will glow GREEN LED to show sensor in wake mode.

Following is a snapshot of demo application output on serial terminal:

 ISSDK MMA865x sensor example demonstration for Auto Wake Sleep feature.
 Successfully Initialized Sensor
 Successfully Applied MMA865x Sensor Configuration

 MMA865x is now active and detecting sudden motion gesture of lifting up the board...

 Motion Detected!!!
 Total Motion Detection Events captured = 1
 Will go to auto-sleep if no-motion detected until ASLP expiry

 No-Motion Detected - ASLP Counter = 6sec Expired
 Going to Sleep Mode...

 Motion Detected!!!
 Total Motion Detection Events captured = 2
 Will go to auto-sleep if no-motion detected until ASLP expiry

 No-Motion Detected - ASLP Counter = 6sec Expired
 Going to Sleep Mode...
