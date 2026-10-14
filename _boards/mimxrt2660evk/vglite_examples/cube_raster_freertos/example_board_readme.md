Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT2660-EVK board
- Personal Computer
- RK055MHD091A0 MIPI panel (not necessary if using other panel)

Board settings
============
Connect the appropriate display panel to the EVK display connector for the
selected panel option.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the debug USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, a cube with different images on each side is rendered
on the screen. The cube slowly rotates. A debug message is printed on the serial
terminal:

    60 frames in 1170 mSec: 51.282 FPS
