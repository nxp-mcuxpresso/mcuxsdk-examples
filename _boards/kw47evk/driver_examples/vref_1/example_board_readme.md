Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
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
ADC conversion result: 20665
Expected voltage on VREF_OUT: 1.000V
Actual voltage on VREF_OUT: 1.006V

Use trim value: 1
ADC conversion result: 22726
Expected voltage on VREF_OUT: 1.100V
Actual voltage on VREF_OUT: 1.106V

Use trim value: 2
ADC conversion result: 24827
Expected voltage on VREF_OUT: 1.200V
Actual voltage on VREF_OUT: 1.208V

Use trim value: 3
ADC conversion result: 26915
Expected voltage on VREF_OUT: 1.300V
Actual voltage on VREF_OUT: 1.310V

Use trim value: 4
ADC conversion result: 28968
Expected voltage on VREF_OUT: 1.400V
Actual voltage on VREF_OUT: 1.410V

Use trim value: 5
ADC conversion result: 31041
Expected voltage on VREF_OUT: 1.500V
Actual voltage on VREF_OUT: 1.511V

Use trim value: 6
ADC conversion result: 33173
Expected voltage on VREF_OUT: 1.600V
Actual voltage on VREF_OUT: 1.615V

Use trim value: 7
ADC conversion result: 35173
Expected voltage on VREF_OUT: 1.700V
Actual voltage on VREF_OUT: 1.712V

Use trim value: 8
ADC conversion result: 37292
Expected voltage on VREF_OUT: 1.800V
Actual voltage on VREF_OUT: 1.815V

Use trim value: 9
ADC conversion result: 39309
Expected voltage on VREF_OUT: 1.900V
Actual voltage on VREF_OUT: 1.913V

Use trim value: 10
ADC conversion result: 41332
Expected voltage on VREF_OUT: 2.000V
Actual voltage on VREF_OUT: 2.012V

Use trim value: 11
ADC conversion result: 43384
Expected voltage on VREF_OUT: 2.100V
Actual voltage on VREF_OUT: 2.112V

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
