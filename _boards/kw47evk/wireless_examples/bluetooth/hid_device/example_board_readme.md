Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Hardware references
=====================

| Platform                 | ADVSW       | CONNLED   |
| ------------------------ | ----------- | --------  |
| KW47-EVK                 | SW2         | LED1      |

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
The HID mouse can be connected to any Bluetooth Smart Ready products available on the market that
support HID devices or to another supported platform running the HID Host example.
To make the HID mouse visible, press the ADVSW button to start sending advertisements, which causes
CONNLED to start flashing. The sensor name “NXP_HID” shows on the device when its scanning is active. A solid CONNLED indicates a successful connection between the 2 devices. When prompted to enter the pin, type the 999999 passkey.
When configured, the HID mouse starts sending HID report, which is configured as explained above, with
notifications every 100 milliseconds. The mouse cursor shows a square pattern movement on the screen.