Hardware requirements
=====================
- Two Micro USB cables
- LPCXpresso55S28 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board for downloading
    the program and supply power for the board, connect another USB cable between host PC and USB port
	on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Open a serial terminal(like putty) to communicate with the board via virtual com.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The debug console virtual com will echo back the received characters.