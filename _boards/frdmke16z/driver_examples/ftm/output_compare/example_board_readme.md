Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- An oscilloscope
- Personal Computer

Board settings
==============
Probe oscilloscope at PTD1(J2-7) to see output signal.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~
FTM example for output compare
You will see the output signal toggle
Probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~
- Probe oscilloscope at PTD1(J2-7) to see output signal.