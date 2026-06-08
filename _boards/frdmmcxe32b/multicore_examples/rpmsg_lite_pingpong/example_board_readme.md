Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the RPMsg-Lite pingpong demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RPMsg demo starts
Primary core received a msg
Message: Size=4, DATA = 1
Primary core received a msg
Message: Size=4, DATA = 3
Primary core received a msg
Message: Size=4, DATA = 5
.
.
.
Primary core received a msg
Message: Size=4, DATA = 101

RPMsg demo ends
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note:
The secondary core (M7_1) image is embedded into the primary core (M7_0) image
and programmed to flash at address 0x00600000. The primary core releases the
secondary core through the MC_ME interface and the secondary core runs in place
directly from flash. No copy-to-RAM step is performed.
