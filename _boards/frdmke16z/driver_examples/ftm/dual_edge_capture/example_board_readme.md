Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- Personal Computer

Board settings
==============
Connect input signal to J2-7

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FTM dual-edge capture example
Once the input signal is received the input capture values are printed
The input signal's pulse width is calculated from the capture values & printed

Capture value C(n)V=3

Capture value C(n+1)V=5a8

Input signals pulse width=24 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
