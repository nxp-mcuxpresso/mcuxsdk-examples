Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- Personal Computer

Board settings
==============
Connect input signal to J2-2

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

~~~~~~~~~~~~

PWT example: input signal whose pulse width needs to be measured


Toggled Low overflow


Positive pulse width=16 usec


Negative pulse width=11 usec


Toggled High overflow


Positive pulse width=11 usec


Negative pulse width=17 usec

~~~~~~~~~~~~
