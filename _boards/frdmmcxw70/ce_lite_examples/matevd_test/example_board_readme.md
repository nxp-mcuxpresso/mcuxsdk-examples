Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
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
FULL WGN 17 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000003
COMP WGN 17 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000004
FULL LOC 17 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000005
COMP LOC 17 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000006
FULL WGN 18 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000007
COMP WGN 18 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000008
FULL LOC 18 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA000009
COMP LOC 18 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA00000A
FULL WGN 20 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA00000B
COMP WGN 20 CF32 MAT EVD Test Passed: Status=DD000001, Reply=AA00000C
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

