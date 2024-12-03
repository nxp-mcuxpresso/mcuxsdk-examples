Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer
- Loopback network cable RJ45 standard (optional)

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
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ENET example start.
Wait for PHY init...
Wait for PHY link up...

Transmission start now!
The 1 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 2 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 3 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 4 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 5 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 6 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 7 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 8 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 9 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 10 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 11 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 12 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 13 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 14 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 15 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 16 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 17 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 18 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 19 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 20 frame transmitted success!
 One frame received. the length 1014
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff

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