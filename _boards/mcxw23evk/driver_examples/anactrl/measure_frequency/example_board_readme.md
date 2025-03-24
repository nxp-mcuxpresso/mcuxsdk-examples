Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J33.
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
The log below shows example output of the ANACTRL driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Analog control measure frequency example.

Reference clock source: 1 MHz FRO clock.

Target clock source: 32kHz RTC clock.

Target clock frequency: 32242 Hz.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
