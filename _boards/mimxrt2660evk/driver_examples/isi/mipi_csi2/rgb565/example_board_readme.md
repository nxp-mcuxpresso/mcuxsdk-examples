Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK
- Personal Computer
- RK055MHD091A0 MIPI panel
- OV5640 module

Board settings
==============
1. Connect the OV5640 camera module to J94.
2. Connect the RK055MHD091A0 MIPI panel to J19.
4. Use external power instead of internal.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the camera received pictures are shown in the LCD.
