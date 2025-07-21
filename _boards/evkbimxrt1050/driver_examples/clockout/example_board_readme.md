Hardware requirements
=====================
- Mini/micro USB cable
- EVK_BIMXRT1050 board
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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP27;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP28.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	1 -- USB1 PLL Clock(Divided by 2).
	2 -- SYS PLL Clock(Divided by 2).
	3 -- VIDEO PLL Clock(Divided by 2).
	4 -- Semc Clock Root.
	5 -- Lcdif Pix Clock Root.
	6 -- AHB Clock Root.
	7 -- IPG Clock Root.
	8 -- Perclk Root.
	9 -- Ckil Sync Clock Root.
	10 -- PLL4 Main Clock.
1

Please set the clock divider, range from 1 to 8.
1

Theoretically, the output frequency of CLKO1 is 240000000 Hz.
Please Choose one clock to be outputted on CLKO2.
	1 -- Usdhc1 Clock Root.
	2 -- LPI2C Clock Root.
	3 -- CSI Clock Root.
	4 -- OSC Clock.
	5 -- Usdhc2 Clock Root.
	6 -- SAI1 Clock Root.
	7 -- SAI2 Clock Root.
	8 -- SAI3 Clock Root(Shared with ADC1 and ADC2 alt_clk root).
	9 -- Can Clock Root(FlexCAN, shared with CANFD).
	10 -- Flexspi Clock Root.
	11 -- UART Clock Root.
	12 -- Spdif0 Clock Root.
3

Please set the clock divider, range from 1 to 8.
2

Theoretically, the output frequency of CLKO2 is 6000000 Hz.

Please press any key to continue.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
