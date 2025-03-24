Hardware requirements
=====================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
Use the default jumper settings.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the EVK board J33.
2.  Open a serial terminal with the following settings
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build and download the application
4.  Start a debug session or press the reset button

Running the demo
================
The clock can be measured with an oscilloscope on J21-2.

The log below shows the output of the clockout demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please choose one output clock.
	A -- Main Clock.
	B -- CLKIN Clock.
	C -- FRO 32 MHz Clock.
	D -- FRO 1 MHz Clock.
	E -- FRO 24 MHz clock.
	F -- Oscillator 32 kHz Clock.
