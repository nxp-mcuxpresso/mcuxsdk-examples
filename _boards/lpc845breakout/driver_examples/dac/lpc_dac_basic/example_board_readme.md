Hardware requirements
=====================
- Micro USB cable
- LPC845 Breakout board
- Personal Computer

Board settings
==============
PIO0_17 is DAC0 output pin.

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
The log below shows the output of the DAC basic driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DAC basic Example.

Please input a value (0 - 1023) to output with DAC:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Probe the signal of PIO0_17 using an oscilloscope.
