Hardware requirements
=====================
- Micro USB cable
- EVK-MIMX8M Plus board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
============
No special settings are required.

**Please note this application can't support booting by uboot! and accordingly it does not support Flash target!**
This example aims to show the basic usage of the IP's function, some of the used Resources are assigned to Cortex-A core by uboot.

Prepare the Demo
===============
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW3 to power on the board
2.  Connect a USB cable between the host PC and the J23 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Running the demo
===============
When the demo runs successfully, the log would be seen on the debug terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This is ECSPI CMSIS SDMA loopback transfer example.
The ECSPI will connect the transmitter and receiver sections internally.
Start transfer...

  This is ECSPI_MasterSignalEvent_t

Transfer completed! 
ECSPI transfer all data matched! 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
