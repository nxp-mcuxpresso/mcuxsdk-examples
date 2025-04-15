Toolchain supported
===================
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Hardware references
===================
| Platform        | SCANSW      | CONNLED   | ROLESW  |
|:--------------- |:-----------:|:---------:|:-------:|
| FRDM-MCXW23     | SW4         | Blue LED  | SW2     |

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
