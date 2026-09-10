Hardware requirements
=====================
- Micro USB cable (for debug terminal)
- IMX95LPD5-EVK board
- Display connected to LVDS interface
- Cameras connected to CSI
- 12V~20V power supply
- Personal Computer
- (optional) USB-A to USB-A cable for image storing via USB

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
You should see camera output displayed on the LVDS connected display.
You should see hw statistics every second on the debug.
