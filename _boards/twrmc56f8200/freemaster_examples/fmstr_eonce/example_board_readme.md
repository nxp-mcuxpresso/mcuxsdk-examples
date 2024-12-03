Hardware requirements
=====================
- Mini/micro USB cable
- TWR-MC56F8200 board
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
No special settings are required. The demo uses a debugging interface to communicate
with a running MCU application using the JTAG EOnCE RTD Unit. Standard debugger must 
not be active during the FreeMASTER communication.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port (JM60 USB) on the target board.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

Disconnect the debugger
=======================
4.  Terminate the debugger session to release the JTAG interface for FreeMASTER use.
5.  Reset the target board to make sure it runs after detaching the debugger.

Connect with FreeMASTER
=======================
6.  Run FreeMASTER, use the Connection Wizard or open Project Options.
7.  Select communication using the FreeMASTER JTAG EOnCe Communication plug-in for 56F800E.
8.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
9.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
10. The demo is now running, you should be able to watch variable values and graphs.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster
