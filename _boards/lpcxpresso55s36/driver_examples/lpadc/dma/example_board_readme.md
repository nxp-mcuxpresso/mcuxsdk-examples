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
Note: MCUXpresso IDE project default debug console is semihost
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
The log below shows the output of the lpadc dma example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC DMA Example

ADC Full Range: 65536

Please press any key to trigger the conversion.

Adc conversion word : 0x81008BA1

ADC conversion value: 35745

Adc conversion word : 0x81008BE7

ADC conversion value: 35815

Adc conversion word : 0x81008B14

ADC conversion value: 35604

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
