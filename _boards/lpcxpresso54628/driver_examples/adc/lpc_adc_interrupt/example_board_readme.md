Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54628 board
- Personal Computer

Board settings
============
- ADC CH4 input signal J12-2(PIO0-16).

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
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
Press any key from keyboard and trigger the conversion.
The log below shows example output of the ADC interrupt example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADC interrupt example.
ADC Calibration Done.
Configuration Done.
ADC Full Range: XXXX
gAdcResultInfoStruct.result        = 675
gAdcResultInfoStruct.channelNumber = 4
gAdcResultInfoStruct.overrunFlag   = 0

gAdcResultInfoStruct.result        = 676
gAdcResultInfoStruct.channelNumber = 4
gAdcResultInfoStruct.overrunFlag   = 0

gAdcResultInfoStruct.result        = 676
gAdcResultInfoStruct.channelNumber = 4
gAdcResultInfoStruct.overrunFlag   = 0

...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
