Hardware requirements
=====================
- Type-C USB cable
- MCXW70-LOC Board
- Personal Computer

Board settings
============

Prepare the Demo
================
1. Connect a USB cable between the host PC and the LOC board J7.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press anykey to start the example...

EWM example
Press SW1 to expire EWM
EWM interrupt is occurred
Press SW1 to expire EWM again
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

