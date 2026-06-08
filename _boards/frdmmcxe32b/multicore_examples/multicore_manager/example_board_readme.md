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
The log below shows the output of the multicore manager demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello World from the Primary Core!

Starting Secondary core.
The secondary core application has been started.

Press the SW3 button to toggle Secondary core Start/Stop.
When no action is taken the secondary core application crashes intentionally after 100 LED toggles (simulated exception), generating the RemoteExceptionEvent to this core.
Use the Stop and then the Start button to get it running again.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press the SW3 button ("User" on the board silkscreen) once to stop the secondary
core and again to start it. On the FRDM-MCXE32B only a single user button is
wired, so the same button toggles Stop/Start.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
The secondary core (M7_1) image is embedded into the primary core (M7_0) image
and programmed to flash at address 0x00600000. The primary core releases the
secondary core through the MC_ME interface and the secondary core runs in place
directly from flash. No copy-to-RAM step is performed.
