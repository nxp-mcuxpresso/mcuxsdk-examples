Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso51U68 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J6) on the board
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
The log below shows example output of the frequency measure demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Capture source: Watchdog oscillator (main clock reference), reference frequency = 48000000 Hz
Computed frequency value = 603515 Hz
Expected frequency value = 500000 Hz

Capture source: RTC32K oscillator (main clock reference), reference frequency = 48000000 Hz
Computed frequency value = 32226 Hz
Expected frequency value = 32768 Hz

Capture source: FRO oscillator (main clock reference), reference frequency = 48000000 Hz
Computed frequency value = 11997070 Hz
Expected frequency value = 12000000 Hz
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
