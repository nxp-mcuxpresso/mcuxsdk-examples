Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- MCUXpresso10.1.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso824MAX board
- Personal Computer

Board settings
==============
- ADC CH1 input signal J5-1(PIO0-6).

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J3) on the target board.
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC basic example.
ADC Calibration Done.
Configuration Done.
ADC Full Range: XXXX
adcResultInfoStruct.result        = 675
adcResultInfoStruct.channelNumber = 1
adcResultInfoStruct.overrunFlag   = 0

adcResultInfoStruct.result        = 676
adcResultInfoStruct.channelNumber = 1
adcResultInfoStruct.overrunFlag   = 0

adcResultInfoStruct.result        = 675
adcResultInfoStruct.channelNumber = 1
adcResultInfoStruct.overrunFlag   = 0

...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

