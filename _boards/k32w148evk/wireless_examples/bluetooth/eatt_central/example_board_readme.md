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

| Platform                 | SCANSW      | CONNLED   |
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
The application can be tested using another board flashed with the EATT Peripheral application.
After flashing the board, the EATT Central is in idle mode (all LEDs flashing).
To start scanning, press the SCANSW button. To make it enter discoverable mode, perform the same step on the EATT Peripheral board.
After connecting to the EATT Peripheral, CONNLED turns solid. The EATT central performs service discovery, indicates its EATT support to the server by writing the Client Supported Features characteristic, enables indications for services A and B, and then initiates an EATT connection with the server.
After the EATT connection is complete, the console displays the received data and the bearer on which it was sent. To disconnect the node, hold the SW2 button pressed for 2-3 seconds. The node then restarts scanning.