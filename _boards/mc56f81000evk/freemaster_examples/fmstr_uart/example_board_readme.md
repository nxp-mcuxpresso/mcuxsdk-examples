Hardware requirements
=====================
- Mini/micro USB cable
- MC56F81000 EVK board
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port (JM60 USB) on the target board.
    Alternatively, use the USB-to-Serial interface at Boot USB port - see main.c for configuration option.
2.  Compile and download the program to the target board.

Running the demo
================
3.  Run FreeMASTER and use connection wizard to connect at 115200 bps.
4.  Note that unlike the MC56F83000 EVK case, this target microcontroller's Flash 
    is not large enough to store the full example project file, so you have 
    to open it from a local hard drive. Locate the "example.pmp" file in the 
    SDK package (in the freemaster_examples/common directory) and open it in
    the FreeMASTER tool.
5.  Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
