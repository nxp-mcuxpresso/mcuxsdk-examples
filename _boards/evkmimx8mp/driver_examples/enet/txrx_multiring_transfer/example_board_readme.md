Hardware requirements
=====================
- Micro USB cable
- EVK-MIMX8M Plus board
- J-Link Debug Probe
- 12V power supply
- Loopback network cable RJ45 standard
- Personal Computer

Board settings
============
J9 connected with loopback network cable.

**Please note this application can't support running with Linux BSP!**
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.
If using the U-Boot bootaux to boot the Mcore, the U-Boot from Linux release 5.15.52_2.1.0 or later should be used. 


Prepare the Demo
===============
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board
2.  Connect a USB cable between the host PC and the J23 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert loopback network cable into the board's Ethernet RJ45 port.
5.  Download the program to the target board.
6.  Launch the debugger in your IDE to begin running the demo.

Make loopback network cable:
      Standard 1         Standard 2
J1    orange+white       green+white
J2    orange             green
J3    green+white        orange+white
J4    blue               brown+white
J5    blue+white         brown
J6    green              orange
J7    brown+white        blue
J8    brown              blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to the sheet above. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

Running the demo
================
When the demo runs, the log would be seen on the debug terminal like:

ENET multi-ring txrx example start.
The 0 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 1, 0, 0 now!
The 1 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 1, 0, 1 now!

......

The 10 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 10, 9, 10 now!
The 10 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 10, 10, 10 now!
30 frame transmitted succeed!
10 frame successfully received from the ring 0!
10 frame successfully received from the ring 1!
10 frame successfully received from the ring 2!
