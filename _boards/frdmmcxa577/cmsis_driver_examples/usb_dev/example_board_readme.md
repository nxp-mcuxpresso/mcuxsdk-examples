Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a Type-C USB cable between the PC host and the debug USB port on the board.
2. Connect a second USB cable between the PC host and the USB HS device port on the board.
3. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The CMSIS-Driver Validation framework runs the USB Device driver tests and prints results
to the serial terminal. A passing run looks similar to:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMSIS-Driver Validation
USB Device Driver

GetCapabilities            PASSED
Initialization             PASSED
PowerControl               PASSED
CheckInvalidInit           PASSED

Test Summary: 4 Tests, 4 Passed, 0 Failed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
