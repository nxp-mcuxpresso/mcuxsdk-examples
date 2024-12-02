Hardware requirements
=====================
- Mini/micro USB cable
- Target microcontroller board
- Personal Computer
- FreeMASTER 2.5 or later installed on PC

Board settings
==============
No special settings are required. The demo communicates by uploading and
downloading communication frames directly to/from the microcontroller
RAM buffer using the Debug USB port. Standard debugger must not be active 
during the FreeMASTER communication.

Prepare the demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port on the target board.
2.  Use one of the flash-based build targets.
3.  Compile and download the program to the target microcontroller.
4.  Run and resume application execution when debugger stops in the main() function.

Disconnect the debugger
=======================
5.  Terminate the debugger session to release the JTAG interface for FreeMASTER use.
6.  Reset the target board to make sure it runs after detaching the debugger.

Connect with FreeMASTER
=======================
7.  Run FreeMASTER, use the Connection Wizard or open Project Options.
8.  Select communication using the FreeMASTER Packet Driven BDM Communication (PD-BDM) plug-in.
9.  Configure the PD-BDM plug-in:
     - Use the same "underlying" BDM/JTAG interface as used in debugger (typically CMSIS-DAP).
     - Configure this BDM/JTAG interface and test its connection.
     - Specify or search for a RAM address of the "_pdbdm" communication buffer.
     - Test connection and close the configuration dialog.
10.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
11. Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
12. The demo is now running, you should be able to watch variable values and graphs.

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
