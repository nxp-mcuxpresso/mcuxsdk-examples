Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N5XX-EVK board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J5) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
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

Copy Secondary core image to address: 0x2004e000, size: 3280
Starting Secondary core.
The secondary core application has been started.

Press the SW3 button to Stop Secondary core.
Press the SW4 button to Start Secondary core again.
When no action is taken the secondary core application crashes intentionally after 100 LED toggles (simulated exception), generating the RemoteExceptionEvent to this core.
Use the Stop and then the Start button to get it running again.

.
.
.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note:
The "Copy Secondary core image to address..." log message is not displayed on the terminal window when MCUXpresso IDE is used.
In case of MCUXpresso IDE the secondary core image is copied to the target memory during startup automatically.
