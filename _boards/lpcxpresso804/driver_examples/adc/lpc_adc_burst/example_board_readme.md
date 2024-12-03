Toolchain supported
===================
- IAR embedded Workbench 8.22.1
- Keil MDK 5.24a
- MCUXpresso10.2.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso804 board
- Personal Computer

Board settings
==============
- ADC CH7 input signal CN6-11(PIO0-10).

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
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
ADC burst example.
ADC Calibration Done.
Configuration Done.
ADC Full Range: XXXX
gAdcResultInfoStruct.result        = 547
gAdcResultInfoStruct.channelNumber = 7

gAdcResultInfoStruct.result        = 549
gAdcResultInfoStruct.channelNumber = 7

gAdcResultInfoStruct.result        = 551
gAdcResultInfoStruct.channelNumber = 7

...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

