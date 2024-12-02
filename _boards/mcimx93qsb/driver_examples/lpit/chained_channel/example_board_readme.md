Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-QSB board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.


Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
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
When the example runs successfully, you will see the similar information from the terminal shown as below.
Detail prints for each channel depends on by the total numbs of chained channels and the order of 
interrupt flag handler process for each channel. 

~~~~~~~~~~~~~~~~~~~~~
Starting channel No.0 ...
Starting channel No.1 ...
Starting channel No.2 ...
Starting channel No.3 ...

 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !

 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !
......................................
 Channel No.1 Chained with Channel No.0 interrupt is occurred !

 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.2 Chained with Channel No.1 interrupt is occurred !
......................................
......................................
 Channel No.3 Chained with Channel No.2 interrupt is occurred !
 .....................................
 .....................................
 .....................................

~~~~~~~~~~~~~~~~~~~~~
