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
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
The log below shows example output of the ADC DMA example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADC DMA example.
ADC Calibration Done.
Configuration Done.
ADC Full Range: XXXX
Type in any key to trigger the conversion ...
Conversion word : 0x80022A70
Conversion value: 679

Conversion word : 0x80022930
Conversion value: 659

Conversion word : 0x800228F0
Conversion value: 655

Conversion word : 0x80022920
Conversion value: 658

Conversion word : 0x80022920
Conversion value: 658

...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
