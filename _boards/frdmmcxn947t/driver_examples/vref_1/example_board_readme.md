Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947T Board
- Personal Computer

Board settings
==============
Connect J8-28(P4_23) with TP1(VREFO)

Prepare the Demo
================
1. Connect the type-c and mini USB cable between the PC host and the USB ports on the board.
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
ADC conversion result: 19861
Actual voltage on VREF_OUT: 1.000V

Use trim value: 1
ADC conversion result: 21827
Actual voltage on VREF_OUT: 1.099V

Use trim value: 2
ADC conversion result: 23812
Actual voltage on VREF_OUT: 1.199V

Use trim value: 3
ADC conversion result: 25792
Actual voltage on VREF_OUT: 1.299V

Use trim value: 4
ADC conversion result: 27776
Actual voltage on VREF_OUT: 1.399V

Use trim value: 5
ADC conversion result: 29741
Actual voltage on VREF_OUT: 1.498V

Use trim value: 6
ADC conversion result: 31790
Actual voltage on VREF_OUT: 1.601V

Use trim value: 7
ADC conversion result: 33734
Actual voltage on VREF_OUT: 1.699V

Use trim value: 8
ADC conversion result: 35763
Actual voltage on VREF_OUT: 1.801V

Use trim value: 9
ADC conversion result: 37699
Actual voltage on VREF_OUT: 1.898V

Use trim value: 10
ADC conversion result: 39667
Actual voltage on VREF_OUT: 1.997V

Use trim value: 11
ADC conversion result: 41632
Actual voltage on VREF_OUT: 2.096V

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
