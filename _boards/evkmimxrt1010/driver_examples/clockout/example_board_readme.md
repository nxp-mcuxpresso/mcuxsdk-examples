Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP34;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP31.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	1 -- PLL3 SW Clock(Divided by 2).
	2 -- PLL2 Clock(Divided by 2).
	3 -- ENET PLL Clock(Divided by 2).
	4 -- Core Clock Root.
	5 -- IPG Clock Root.
	6 -- Perclk Root.
	7 -- PLL4 Main Clock.
1

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO1 is 120000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	1 -- LPI2C Clock Root.
	2 -- OSC Clock.
	3 -- LPSPI Clock Root.
	4 -- SAI1 Clock Root.
	5 -- SAI3 Clock Root.
	6 -- Trace Clock Root.
	7 -- Flexspi Clock Root.
	8 -- UART Clock Root.
	9 -- Spdif0 Clock Root.
6

Please set the clock divider, range from 1 to 8.
6

Theoretically, the output frequency of CLKO2 is 22000000 Hz.

Please press any key to continue.
~~~~~~~~~~~~~~~~~~~~~