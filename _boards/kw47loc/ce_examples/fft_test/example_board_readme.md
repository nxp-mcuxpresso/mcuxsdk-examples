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
Test 1: CFFT 32 F16 Test Passed: Status=DD000002, Reply=AA000003
Test 2: CFFT 512 F32 Test Passed: Status=DD000003, Reply=AA000004
Test 3: IFFT 1024 F16 Test Passed: Status=DD000004, Reply=AA000005
Test 4: IFFT 256 F32 Test Passed: Status=DD000005, Reply=AA000006
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

