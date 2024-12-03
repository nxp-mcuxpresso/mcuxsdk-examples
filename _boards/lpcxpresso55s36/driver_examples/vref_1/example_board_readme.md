Hardware requirements
=====================
- Micro USB cable
- Personal Computer
- LPCXpresso55S36 board

Board settings
==============
Populate the R555

Connect J12-16(VREF_OUT) with J9-3(PIO0_31,ADC CH8A)

Prepare the Demo
================
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
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
ADC conversion result: 19599
Expected voltage on VREF_OUT: 1.000V
Actual voltage on VREF_OUT: 0.987V

Use trim value: 1
ADC conversion result: 21839
Expected voltage on VREF_OUT: 1.100V
Actual voltage on VREF_OUT: 1.100V

Use trim value: 2
ADC conversion result: 23942
Expected voltage on VREF_OUT: 1.200V
Actual voltage on VREF_OUT: 1.206V

......

Use trim value: 11
ADC conversion result: 41580
Expected voltage on VREF_OUT: 2.100V
Actual voltage on VREF_OUT: 2.094V

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

