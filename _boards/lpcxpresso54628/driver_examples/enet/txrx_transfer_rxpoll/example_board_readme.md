Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso54628 board
- Personal Computer
- Loopback network cable RJ45 standard (optional)

Board settings
============
For LPCXpresso54628 V2.0:JP11 and JP12 1-2 on.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ENET example start.


Transmission start now!
The 1 frame transmitted success!
The 2 frame transmitted success!
The 3 frame transmitted success!
The 4 frame transmitted success!
The 5 frame transmitted success!
The 6 frame transmitted success!
The 7 frame transmitted success!
The 8 frame transmitted success!
The 9 frame transmitted success!
The 10 frame transmitted success!
The 11 frame transmitted success!
The 12 frame transmitted success!
The 13 frame transmitted success!
The 14 frame transmitted success!
The 15 frame transmitted success!
The 16 frame transmitted success!
The 17 frame transmitted success!
The 18 frame transmitted success!
The 19 frame transmitted success!
The 20 frame transmitted success!
 One frame received. the length 64
 Dest Address ff:ff:ff:ff:ff:ff Src Address 00:23:24:9d:33:de
 One frame received. the length 346
 Dest Address ff:ff:ff:ff:ff:ff Src Address 00:23:24:9d:33:de
 One frame received. the length 64
 Dest Address ff:ff:ff:ff:ff:ff Src Address 00:23:24:9d:33:de

...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Optionally, you may use a loop back cable
====================================
Make a loopback network cable:
      568B standard 	 Unknowed standard
J1    orange+white       green+white
J2    orange             green
J3    green+white        orange+white
J4    blue               brown+white
J5    blue+white         brown
J6    green              orange
J7    brown+white        blue
J8    brown              blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to the 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

1.  Add #define EXAMPLE_USES_LOOPBACK_CABLE 1  into app.h and rebuild the example.
2.  Insert loopback network cable to Ethernet RJ45 port.
3.  Run the demo in the same way as described earlier.