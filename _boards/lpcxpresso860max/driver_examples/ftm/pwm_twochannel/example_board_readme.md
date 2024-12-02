Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860MAX board
- An oscilloscope
- Personal Computer

Board settings
==============
The FTM Pins are J1_5 and J1_7.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 9600 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
FTM example to output PWM on 2 channels

You will see a change in LED brightness if an LED is connected to the FTM pin

If no LED is connected to the FTM pin, then probe the signal using an oscilloscope

~~~~~~~~~~~~~~~~~~~~~~~
