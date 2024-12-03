Hardware requirements
=====================
- Mini/micro USB cable
- Target microcontroller board
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
============
No special settings are required. By default this demo communicates over a virtual 
serial line interface of the debugger USB connection. With a proper RS232 transceiver
and pin configuration, it may also connect to microcontroller UART peripheral.

Prepare the demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port on the target board.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

Connect with FreeMASTER
=======================
4.  Run FreeMASTER, use the Connection Wizard or open Project Options.
5.  Select serial communication, choose correct COMx port and connect at speed 115200 bps.
6.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
7.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
8.  The demo is now running, you should be able to watch variable values and graphs.

Note to CM4/CM0+ core execution
===============================
The target MCU is a dual core consisting of Cortex M4 and Cortex M0+ cores. The MCU boots 
by running CM4 core by default, so the cm4 example projects will run without any change.
The cm0plus examples will not run automatically. You should be able to use a debugger 
session (tested in IAR EWARM IDE) to upload the compiled code to MCU RAM and start the 
CM0+ core to execute it. In a typical application, the CM4 core will boot and will set up
the execution environment for the CM0+ core. Alternatively, you can set the CM0+ core to
be the default booting core as described at https://wiki.segger.com/K32W#Boot_ROM.
In any case, make sure that only one of the cores will access the communication peripheral
module and will handle the FreeMASTER communication.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster.
