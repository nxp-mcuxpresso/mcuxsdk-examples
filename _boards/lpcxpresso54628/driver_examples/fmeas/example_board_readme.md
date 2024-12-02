Hardware requirements
=====================
- Micro USB cable
- LPCLPCXpresso54628 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
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
The log below shows example output of the frequency measure demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Capture source: Watchdog oscillator (main clock reference), reference frequency = 220000000 Hz
Computed frequency value = 577392 Hz
Expected frequency value = 500000 Hz

Capture source: RTC32K oscillator (main clock reference), reference frequency = 220000000 Hz
Computed frequency value = 26855 Hz
Expected frequency value = 32768 Hz

Capture source: FRO oscillator (main clock reference), reference frequency = 220000000 Hz
Computed frequency value = 11990966 Hz
Expected frequency value = 12000000 Hz
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
