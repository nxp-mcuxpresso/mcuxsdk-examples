Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
- Personal Computer

Board settings
============
No special settings needed.

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

The example uses 10BASE-T1S digital PHY.

Running the demo
================
The log below shows example output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ENET example start.
Wait for PHY init...
Wait for PHY link up...

Transmission start now!
The 1-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 2-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 3-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 4-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 5-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 6-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 7-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 8-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 9-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 10-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 11-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 12-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 13-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 14-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 15-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 16-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 17-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 18-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 19-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff
The 20-th frame transmitted success!
 One frame received. the length 1018
 Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:ff:ff:ff

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
