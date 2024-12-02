Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso51U68 board
- Personal Computer

Board settings
============
- ADC CH4 input signal J1-15(PIO1-1).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J6) on the board
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
Press any key from keyboard and trigger the conversion.
The log below shows example output of the ADC basic example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADC basic example.
ADC Calibration Done.
Configuration Done.
ADC Full Range: XXXX
adcResultInfoStruct.result        = 675
adcResultInfoStruct.channelNumber = 4
adcResultInfoStruct.overrunFlag   = 0

adcResultInfoStruct.result        = 676
adcResultInfoStruct.channelNumber = 4
adcResultInfoStruct.overrunFlag   = 0

adcResultInfoStruct.result        = 675
adcResultInfoStruct.channelNumber = 4
adcResultInfoStruct.overrunFlag   = 0

...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
