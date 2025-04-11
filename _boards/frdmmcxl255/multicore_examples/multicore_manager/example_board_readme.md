Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board. 
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
The log below shows the output of the hello world multicore demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello World from the Primary Core!

Copy Secondary core image to address: 0xa1000000, size: 4278
Starting Secondary core.
The secondary core application has been started.

Press the SW2 button to toggle Secondary core Start/Stop.
When no action is taken the secondary core application crashes intentionally after 100 LED toggles (simulated exception), generating the RemoteExceptionEvent to this core.
Use the Stop and then the Start button to get it running again.

.
.
.

Secondary core cannot be stopped!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note:
The "Copy Secondary core image to address..." log message is not displayed on the terminal window when MCUXpresso IDE is used.
In case of MCUXpresso IDE the secondary core image is copied to the target memory during startup automatically.

Note:
The FRDM-MCXL255 board does not support stopping of secondary code.
After pressing the button prints that secondary core cannot be stopped.
Only way to start the secondary core again is POR.


Note:
As it is not possible to use any on-board LED from the cm0plus core the secondary core is running LED indication is not working in this example.