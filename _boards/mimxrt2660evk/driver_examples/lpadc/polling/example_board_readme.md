Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
- ADC0 CH0A input signal J94-13(PIO3_0).

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Polling Example
ADC Full Range: 65536
Full channel scale (Factor of 1).
Please press any key to get user channel's ADC value.
ADC value: 10538
ADC value: 10538
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The analog voltage input range is 0-1.8V (VDDA_1V8_DATACONV reference).
