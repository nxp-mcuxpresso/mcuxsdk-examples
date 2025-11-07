Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-ORANGEBOX board
- 12V~20V power supply
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the J12(DEBUG_USB) USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Program flash.bin of this demo to SD/eMMC.
4.  Connect 12V~20V power supply to the board.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC EP0 frame loopback example start.
Wait for PHY link up...
The 1 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 2 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
...
The 19 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 20 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00

NETC Switch frame loopback example start.
Wait for PHY link up...
The 1 frame transmitted success on port 0!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 2 frame transmitted success on port 1!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 3 frame transmitted success on port 2!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
