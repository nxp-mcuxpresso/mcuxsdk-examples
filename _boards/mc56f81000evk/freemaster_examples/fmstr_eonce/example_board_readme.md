Hardware requirements
=====================
- Mini/micro USB cable
- MC56F81000 EVK board
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
9.  Note that unlike the MC56F83000 EVK case, this target microcontroller's Flash 
    is not large enough to store the full example project file, so you have 
    to open it from a local hard drive. Locate the "example.pmp" file in the 
    SDK package (in the freemaster_examples/common directory) and open it in
    the FreeMASTER tool.
10. The demo is now running, you should be able to watch variable values and graphs.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster
