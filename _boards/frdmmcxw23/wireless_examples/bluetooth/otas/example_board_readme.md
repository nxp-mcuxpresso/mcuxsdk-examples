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
|  Platform   | ADVSW | SCANSW | CONNLED  |
|-------------|-------|--------|----------|
| FRDM-MCXW23 | SW4   | SW4    | Blue LED |

>Note: MCXW235 SoC variant is not supported.

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Start the OTAP application from and follow the instructions in the manual.
    Note that in case the Over-the-Air Programming tool is used from the NXP Connectivity Test Suite, the "QN9080" profile has to be used when converting the binary file.