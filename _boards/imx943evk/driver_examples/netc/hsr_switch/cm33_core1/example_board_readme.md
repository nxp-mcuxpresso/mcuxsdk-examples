Hardware requirements
=====================
- Mini/micro USB cable
- imx943evk board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the USB DBG port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Program flash.bin to boot source and start the board.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC Switch frame loopback example start.
Wait for PHY link up...

 Configure HSR!

 Press any key to start send a broadcast frame
The frame transmitted success!
 A frame received. The length is 120  Dest Address 33:33:00:00:00:16 Src Address be:45:c8:9c:58:ad
 A frame received. The length is 80  Dest Address 33:33:00:00:00:02 Src Address 00:04:9f:08:49:3e
 A frame received. The length is 120  Dest Address 33:33:00:00:00:16 Src Address be:45:c8:9c:58:ad
 A frame received. The length is 272  Dest Address 01:80:c2:00:00:0e Src Address be:45:c8:9c:58:ad
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
