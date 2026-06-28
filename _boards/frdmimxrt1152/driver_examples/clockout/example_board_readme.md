Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============

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
To measure the output signal of CLKO1, please use the oscilloscope to probe TP42;
To measure the output signal of CLKO2, please use the oscilloscope to probe TP40.
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please Choose one clock to be outputted on CLKO1.
	1 -- OSC_RC_48M_DIV2.
	2 -- OSC_24M.
	3 -- OSC_RC_400M.
	4 -- OSC_RC_16M.
	5 -- SYS_PLL2_PFD2.
	6 -- SYS_PLL2_CLK.
	7 -- SYS_PLL3_PFD1.
	8 -- SYS_PLL1_DIV5.
5

Please set the clock divider, range from 1 to 256.
5

Theoretically, the output frequency of CLKO1 is 105600000 Hz.
Please Choose one clock to be outputted on CLKO2.
	1 -- OSC_RC_48M_DIV2.
	2 -- OSC_24M.
	3 -- OSC_RC_400M.
	4 -- OSC_RC_16M.
	5 -- SYS_PLL2_PFD3.
	6 -- OSC_RC_48M.
	7 -- SYS_PLL3_PFD1.
	8 -- AUDIO_PLL_CLK.
5

Please set the clock divider, range from 1 to 256.
5

Theoretically, the output frequency of CLKO2 is 9600000 Hz.

Please press any key to continue.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
