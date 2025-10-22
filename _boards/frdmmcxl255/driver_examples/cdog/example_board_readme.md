Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
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

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CDOG Peripheral Driver Example

CDOG IRQ Reached
* Miscompare fault occured *

CDOG IRQ Reached
* Sequence fault occured *

intruction timer:  ffffd9
intruction timer:  fc382c
intruction timer:  f86fc3
intruction timer:  f4a75e
...
intruction timer:  11afd1
intruction timer:   de76c
intruction timer:   a1f0a
intruction timer:   6569c
intruction timer:   28e3e
* Timeout fault occured *

CDOG IRQ Reached
End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~