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
The application is built to work only with the HID Device application. It supports up to 2 peripherals connected at the same time.

1. Open a serial port terminal and connect it to board. The start screen is displayed after the board is reset.
2. To start scanning for devices, press the SCANSW button on the HID Host board. To make it enter
discoverable mode, perform the same step on the HID device board. The host connects with the board after
it sees it advertise the HID service, connects to it, and configures report notifications. The device then starts
sending HID reports.
3. To connect a second HID device, press again the ADVSW button on the HID Host board to start scanning
for devices. Do the same on the second HID device board to make it enter discoverable mode. The host
connects with the board after it sees it advertise the HID service, connects to it, and configures report
notifications. The device then starts sending HID reports. The console displays reports from both devices.