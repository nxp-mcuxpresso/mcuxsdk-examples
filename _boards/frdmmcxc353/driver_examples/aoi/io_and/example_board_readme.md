Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
==============
IO0     = XBAR_IN8 (P1_1, J4_2),
IO1     = XBAR_IN7 (P2_17, J2_2/SW3),
AOI_OUT = XBAR_OUT11 (P2_3 LED_GREEN).

AOI_OUT = IO0 & IO1

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When IO0 and IO1 are both high level, the AOI_OUT is high level.
When any of IO is low level, the AOI_OUT is low level.

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
aoi_io_and project.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
