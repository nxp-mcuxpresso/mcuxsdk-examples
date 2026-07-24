Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer
- Display panel, one of:
    - LCD-PAR-S035 8080 parallel panel (480 x 320) for the DBI interface
    - LCM-RGB-5INCH-CT panel (800 x 480) for the DPI interface

Board settings
==============
Connect the display panel matching the interface selected in dcif_support.h
(DEMO_INTERFACE_TYPE: DBI = LCD-PAR-S035, DPI = LCM-RGB-5INCH-CT) to the board's
parallel-LCD connector <TODO: confirm connector, e.g. Jxx>.

Prepare the Demo
================
1.  Connect a Type-C USB cable between the host PC and the OpenSDA USB port on the target board.
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
When the example runs, the screen shows what is described in the example overview.
