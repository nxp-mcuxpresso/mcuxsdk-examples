Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
- ADC0 CH0A input signal J94-13 (PIO3_0).
- ADC0 CH1B input signal J94-16 (PIO3_3).

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
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Triger Dual Channel Example
ADC Full Range: 65536
Please press any key to get user channel's ADC value.

ADC channal A value: 35822
ADC channal B value: 35826

ADC channal A value: 35734
ADC channal B value: 35763

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The analog voltage input range is theoretically 0-1.8V. However, due to the workaround for ERR051152, the maximum voltage should be limited to 1.78V.
Due to the errata051385, the ADC reference voltage is connect to the VDDA_ADC_1P8.
