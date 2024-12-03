Hardware requirements
=====================
- Micro USB cable
- EVK-MIMX8M Plus board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
============
Example uses phy looback. No need to connect any network cable on port.

**Please note this application can't support running with Linux BSP!**
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
===============
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a micro USB cable between the host PC and the J19 USB Debug Port on the CPU board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
===============
When the demo runs, the log would be seen on the terminal like:

ENET example start.
 Get the 1-th time 0 second, 1864 nanosecond
 Get the 2-th time 0 second, 7275192 nanosecond
 Get the 3-th time 0 second, 14804820 nanosecond
 Get the 4-th time 0 second, 22420658 nanosecond
 Get the 5-th time 0 second, 30036030 nanosecond
 Get the 6-th time 0 second, 37651402 nanosecond
 Get the 7-th time 0 second, 45267240 nanosecond
 Get the 8-th time 0 second, 52882612 nanosecond
 Get the 9-th time 0 second, 60498450 nanosecond
 Get the 10-th time 0 second, 68113822 nanosecond

Transmission start now!
The 1 frame transmitted success!
 the timestamp is 0 second, 167770756 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 167973396 nanosecond
The 2 frame transmitted success!
 the timestamp is 0 second, 200548167 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 200750756 nanosecond
The 3 frame transmitted success!
 the timestamp is 0 second, 233324728 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 233527436 nanosecond
The 4 frame transmitted success!
 the timestamp is 0 second, 266102139 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 266305136 nanosecond
The 5 frame transmitted success!
 the timestamp is 0 second, 298878785 nanosecond

......
The 20 frame transmitted success!
 the timestamp is 0 second, 792377480 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 792580698 nanosecond
