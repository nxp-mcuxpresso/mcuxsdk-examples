Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
- ADC CH0A input signal J2-9 (PIO0_23).

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the LPC-Link USB port (J16) on the target board.
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

The log below shows example output of the LPADC INTERRUPT driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Interrupt Example
ADC Full Range: 65536
Please press any key to get user channel's ADC value.
ADC value: 35852
ADC interrupt count: 1
ADC value: 35586
ADC interrupt count: 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
