Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
U38 DIP 1,2,3,4 all off
HD12 1-2, 3-4 connected

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
    For IAR debug/release build, please ensure the image was written to flash offset 0x400 before running.

Running the demo
================
The log below shows example output of the WWDT driver demo in the terminal window:

--- Time out reset test start ---
Watchdog reset occurred

--- Window mode refresh test start ---
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
 WDOG has been refreshed!
