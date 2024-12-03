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
Status=       0, Reply=AA000001
OneCmd Non-blocking F32 MAT MULT Test Passed: Status=       0, Reply=AA000003
OneCmd Non-blocking CF32 MAT MULT Test Passed: Status=       0, Reply=AA000004
Non-blocking Cmd Queue Test Passed: Status=       0, Reply=AA000005
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

