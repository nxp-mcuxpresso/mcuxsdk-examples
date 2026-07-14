Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

Board settings
==============
AOI_OUT = EXT_TRIG_OUT3 PTB5 J19-10
IO0     = EXT_TRIG_IN10 PTB4 J19-9
IO1     = EXT_TRIG_IN11 PTA0 J24-1
AOI_OUT = IO0 & IO1

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
aoi_io_and project.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When IO0 and IO1 are both high level, the AOI_OUT is high level.
When any of IO is low level, the AOI_OUT is low level.
