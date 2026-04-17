Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA287 board
- Personal Computer

Board settings
============
AOI_OUT = EXT_TRIG_OUT3 P4_5 J6-5
IO0     = EXT_TRIG_IN10 P5_5 J2-15
IO1     = EXT_TRIG_IN11 P5_7 J6-2

AOI_OUT = IO0 & IO1

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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

The log below shows the output of the aoi demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AOI_io_and project.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When IO0 and IO1 are both high level, the AOI_OUT is high level.
When any of IO is low level, the AOI_OUT is low level.