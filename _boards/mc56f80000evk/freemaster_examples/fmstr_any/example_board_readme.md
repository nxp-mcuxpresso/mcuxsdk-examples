Hardware requirements
=====================
- Target microcontroller board
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
By default, the demo is configured for standard Serial (UART) communication. It may
be changed to other interface in MCUXpresso SDK ConfigTools. Such other communication 
interfaces may require different physical connections made on board, such as CAN 
transceiver or direct MCU's USB port. Refer to readme.txt files in other FreeMASTER 
demos which are available for this platform to see more details about board setup.

Prepare the demo
===============
1.  Interconnect the host PC and the target microcontroller board using the selected 
    interface cable.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

Connect with FreeMASTER
=======================
4.  Run FreeMASTER, use the Connection Wizard or open Project Options.
5.  Configure Serial, CAN or other communication as required by this demo setup.
6.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
7.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
8.  The demo is now running, you should be able to watch variable values and graphs.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster
