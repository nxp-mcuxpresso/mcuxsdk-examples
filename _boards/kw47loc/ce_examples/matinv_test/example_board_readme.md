Hardware requirements
=====================
- Type-C USB cable
- KW47-LOC Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the LOC board J3.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Status=DD000001, Reply=AA000002
CF32 MAT Inv Test Passed: Status=DD000002, Reply=AA000003
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

