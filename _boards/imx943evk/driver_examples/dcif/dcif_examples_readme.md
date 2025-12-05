Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- BOE EV121WXM-N12 (LCD SPEC) panel

Board settings
============
Connect the LCD SPEC panel to J38

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

For figure_display function:
Load the figure binary file by using Jlink command "loadbin xxx.bin 0x80000000"(default LCD SPEC panel).

Running the demo
================
When the example runs, the screen shows what described in overview.
