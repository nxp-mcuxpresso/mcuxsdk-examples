Hardware requirements
=====================
- FRDMK32L3A6 board
- Personal Computer
- FreeMASTER 3.0 or later installed on PC
- Serial line or USB equipment as needed to connect over selected interface.

Board settings
============
By default, the demo is configured for standard serial (UART) communication. It may
be changed to other interface in MCUXpresso ConfigTools. Run the MCUXpresso ConfigTools, 
select the Peripherals tool and open the Middleware/FreeMASTER component configuration.
Refer to the list of recommended communication options below and make the selection in 
the FreeMASTER configuration page. Note that the other communication interfaces may require
to use different physical connections, such as serial or USB ports. 

Recommended communication options 
=================================
- 'SERIAL' at LPUART0 module connected to virtual serial port of the debugger USB interface.
- 'Packet-driven BDM' using the debugger interface (use flash-based build targets only).

Prepare the demo
===============
1.  Interconnect the host PC and the target microcontroller board using the selected 
    interface cable.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

If using the Packet-driven BDM interface
========================================
4.  Terminate the debugger session to release the JTAG interface for FreeMASTER use.
5.  Reset the target board to make sure it runs after detaching the debugger.

Connect with FreeMASTER
=======================
6.  Run FreeMASTER, use the Connection Wizard or open Project Options.
7.  Configure Serial or other communication as required by this demo setup.
8.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
9.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
10. The demo is now running, you should be able to watch variable values and graphs.

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
