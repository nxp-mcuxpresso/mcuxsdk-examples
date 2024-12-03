Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
- Personal Computer

Board settings
============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The application is shell-based and it is to be used together with a Digital Key Car Anchor demo.

For details on how to exercise the Digital Key scenarios as well as proprietary features such as Connection Handover, please consult AN12971 "Bluetooth Low Energy CCC Digital Key R3 Application Note".