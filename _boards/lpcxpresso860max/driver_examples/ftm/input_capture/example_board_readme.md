Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

Board settings
==============
The FTM input pin is J1_7.

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
FTM input capture example

Once the input signal is received the input capture value is printed

Capture value C(n)V=baaa
~~~~~~~~~~~~~~~~~~~~~~~
