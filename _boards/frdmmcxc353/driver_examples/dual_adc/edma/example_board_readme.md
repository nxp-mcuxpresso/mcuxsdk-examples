Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
============
- ADC0_A5(J4-8), ADC0_B5(J4-6), ADC0_A11(J4-4), ADC0_B11(J4-2) input signal.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DualADC eDMA Example
ADC Full Range: 4096
Please press any key to get user channel's ADC value.
ADC value: 367 1 0 4094
ADC value: 366 2 0 4095
ADC value: 364 1 0 4094
ADC value: 367 1 0 4094
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



