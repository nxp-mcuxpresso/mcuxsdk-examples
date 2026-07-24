Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
- Input A: J94-13 (PIO3_0, XBAR0_INOUT00) -> XBAR1 -> EVTG0_INA0
- Input B: J94-14 (PIO3_1, XBAR0_INOUT01) -> XBAR1 -> EVTG0_INB0
- Output:  EVTG0_OUTA0 -> XBAR0 -> PIO2_26 (XBAR1_INOUT26) -> LED

EVTG logic: LED = INA AND INB

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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

The log below shows the output of the EVTG demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EVTG project.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When IO0 and IO1 are both high level, the EVTG_OUT is high level.
When any of IO is low level, the EVTG_OUT is low level.