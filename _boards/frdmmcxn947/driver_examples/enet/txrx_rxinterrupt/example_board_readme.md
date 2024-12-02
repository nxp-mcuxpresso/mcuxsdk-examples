Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer
- Loopback network cable RJ45 standard

Board settings
============
Set JP13 2-3 on.
Populate R274 to sync reference clock.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Make loopback network cable:
      568B standard 	 Unknowed standard
J1    orange+white       green+white
J2    orange             green
J3    green+white        orange+white
J4    blue               brown+white
J5    blue+white         brown
J6    green              orange
J7	  brown+white        blue
J8    brown              blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

Running the demo
================
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ENET example start.
Wait for PHY init...
Wait for PHY link up...

Transmission start now!
The 1-th frame transmitted success!
1 frame has been successfully received
The 2-th frame transmitted success!
2 frame has been successfully received
The 3-th frame transmitted success!
3 frame has been successfully received
The 4-th frame transmitted success!
4 frame has been successfully received
The 5-th frame transmitted success!
5 frame has been successfully received
The 6-th frame transmitted success!
6 frame has been successfully received
The 7-th frame transmitted success!
7 frame has been successfully received
The 8-th frame transmitted success!
8 frame has been successfully received
The 9-th frame transmitted success!
9 frame has been successfully received
The 10-th frame transmitted success!
10 frame has been successfully received
The 11-th frame transmitted success!
11 frame has been successfully received
The 12-th frame transmitted success!
12 frame has been successfully received
The 13-th frame transmitted success!
13 frame has been successfully received
The 14-th frame transmitted success!
14 frame has been successfully received
The 15-th frame transmitted success!
15 frame has been successfully received
The 16-th frame transmitted success!
16 frame has been successfully received
The 17-th frame transmitted success!
17 frame has been successfully received
The 18-th frame transmitted success!
18 frame has been successfully received
The 19-th frame transmitted success!
19 frame has been successfully received
The 20-th frame transmitted success!
20 frame has been successfully received

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
