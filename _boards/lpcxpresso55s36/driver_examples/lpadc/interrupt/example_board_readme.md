Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============
- ADC CH0A input signal J7-1(PIO1_9).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Interrupt Example
ADC Full Range: 65536
Please press any key to get user channel's ADC value.
ADC value: 35852
ADC interrupt count: 1
ADC value: 35586
ADC interrupt count: 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
