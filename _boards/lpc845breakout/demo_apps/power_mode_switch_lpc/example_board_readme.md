Hardware requirements
=====================
- Micro USB cable
- LPC845 Breakout board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows example output of the power_mode_switch_lpc demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power mode switch Demo for LPC8xx.

Select an option
	1. Sleep mode
	2. Deep Sleep mode
	3. Power Down mode
	4. Deep power down mode
/* after select power mode, terminal will output */
Select wakeup source
    1. Wkt timer
    2. K3, wakeup key
    3. K2, reset key
/* after wakeup, terminal will output */
Wakeup.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
