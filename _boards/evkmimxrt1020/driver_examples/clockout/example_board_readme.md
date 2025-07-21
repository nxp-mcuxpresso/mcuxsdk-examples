Hardware requirements
=====================
- Mini/micro USB cable
- EVK_MIMXRT1020 board
- Personal Computer

Board settings
============
Connect pin:
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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP11;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP12.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	1 -- PLL3 SW Clock.
	2 -- PLL2(divided by 2).
	3 -- ENET PLL(divided by 2).
	4 -- Semc Clock Root.
	5 -- AHB Clock Root.
	6 -- IPG Clock Root.
	7 -- Perclk Root.
	8 -- PLL4 Main Clock.
1

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO1 is 120000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	1 -- USDHC1 Clock Root.
	2 -- LPI2C Clock Root.
	3 -- OSC Clock.
	4 -- LPSPI Clock Root.
	5 -- USDHC2 Clock Root.
	6 -- SAI1 Clock Root.
	7 -- SAI2 Clock Root.
	8 -- SAI3 Clock Root.
	9 -- Trace Clock Root.
	10 -- Can Clock Root.
	11 -- Flexspi Clock Root.
	12 -- UART Clock Root.
	13 -- Spdif0 Clock Root.
4

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO2 is 52800000 Hz.

Please press any key to continue.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
