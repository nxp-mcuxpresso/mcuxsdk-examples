Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
No special settings are required.

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
Note: LPTMR hardware block of FRDM-KE15Z board will trigger a periodic interrupt after every 0.5 second instead of 1 second as other boards.
When the example runs successfully, the following message is displayed in the terminal and you will find the LED RED is taking turns to shine.

~~~~~~~~~~~~~~~~~~~~~
Low Power Timer Example
LPTMR interrupt No.1
LPTMR interrupt No.2
LPTMR interrupt No.3
....................
....................
~~~~~~~~~~~~~~~~~~~~~
