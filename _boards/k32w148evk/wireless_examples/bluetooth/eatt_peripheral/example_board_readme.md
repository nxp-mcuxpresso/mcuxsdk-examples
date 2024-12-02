Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- K32W148-EVK Board
- Personal Computer

Hardware references
=====================

| Platform                 | ADVSW       | CONNLED   |
| ------------------------ | ----------- | --------  |
| K32W148-EVK              | SW2         | LED2      |

Board settings
============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the board.
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
After flashing the board, the peripheral is in idle mode (all LEDs flashing). To start advertising, press the
ADVSW button. When in GAP Discoverable Mode, CONNLED is flashing. When a central node connects
to the peripheral, CONNLED turns solid. The node then waits for an EATT connection request and for the
client to configure indications for the two services. The service information is sent over enhanced bearers only.
The values indicated are cycled between 0 and 10. To disconnect the node, hold the ADVSW button for 2-3
seconds. The node then re-enters GAP Discoverable Mode and starts advertising.