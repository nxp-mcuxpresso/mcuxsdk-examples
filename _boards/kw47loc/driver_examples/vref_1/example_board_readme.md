Hardware requirements
=====================
- Type-C USB cable
- KW47-LOC Board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the LOC board J3.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
VREF example
ADC Full Range: 65536
Default (Factory) trim value is :15

Use trim value: 0
ADC conversion result: 21607
Actual voltage on VREF_OUT: 1.006V

Use trim value: 1
ADC conversion result: 23732
Actual voltage on VREF_OUT: 1.104V

Use trim value: 2
ADC conversion result: 25934
Actual voltage on VREF_OUT: 1.207V

Use trim value: 3
ADC conversion result: 28097
Actual voltage on VREF_OUT: 1.308V

Use trim value: 4
ADC conversion result: 30306
Actual voltage on VREF_OUT: 1.410V

Use trim value: 5
ADC conversion result: 32433
Actual voltage on VREF_OUT: 1.509V

Use trim value: 6
ADC conversion result: 34704
Actual voltage on VREF_OUT: 1.615V

Use trim value: 7
ADC conversion result: 36801
Actual voltage on VREF_OUT: 1.713V

Use trim value: 8
ADC conversion result: 39008
Actual voltage on VREF_OUT: 1.815V

Use trim value: 9
ADC conversion result: 41139
Actual voltage on VREF_OUT: 1.915V

Use trim value: 10
ADC conversion result: 43264
Actual voltage on VREF_OUT: 2.013V

Use trim value: 11
ADC conversion result: 45413
Actual voltage on VREF_OUT: 2.113V

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
