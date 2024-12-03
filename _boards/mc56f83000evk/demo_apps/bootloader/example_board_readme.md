Hardware requirements
=====================
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J19 pin2 and pin3
   > Connect J12 pin1 and pin2
   > Connect J13 pin1 and pin2
   > Connect J15 pin1 and pin2
   > Connect J16 pin2 and pin3
   > Connect J17 pin1 and pin2
2. Onboard OSJTAG debugger setting:
   > Leave J5 open
   > Leave J6 open
     Note: J6 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J11 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J10), leave all J11 pins open.

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.

Running the demo
================
The log below shows the output of the bootloader demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Reset Reason: External pin
Boot from   : flash
External pin reset, reboot from bootloader

Reset Reason: Software reset
Boot from   : bootloader
Usage:
  - Input 'f' to switch fast mode without bootloader involved!
  - Input 'F' to switch fast mode with bootloader involved!
  - Input 'n' to switch normal mode without bootloader involved!
  - Input 'N' to switch normal mode with bootloader involved!
  - Input 'b' to software reset without bootloader involved!
  - Input 'B' to software reset with bootloader involved!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Input character in terminal window according hint, say after input 'b', log shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Software reset, reboot from flash

Reset Reason: Software reset
Boot from   : flash
Usage:
  - Input 'f' to switch fast mode without bootloader involved!
  - Input 'F' to switch fast mode with bootloader involved!
  - Input 'n' to switch normal mode without bootloader involved!
  - Input 'N' to switch normal mode with bootloader involved!
  - Input 'b' to software reset without bootloader involved!
  - Input 'B' to software reset with bootloader involved!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~