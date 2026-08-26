Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal computer

Board settings
==============

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note
====
IAR EWARM cannot debug the RT2660 directly. Use SEGGER Ozone to download and
debug the IAR-built image instead.
