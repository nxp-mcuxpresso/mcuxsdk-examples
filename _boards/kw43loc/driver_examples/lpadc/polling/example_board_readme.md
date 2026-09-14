Hardware requirements
=====================
- Type-C USB cable
- KW43-LOC Board
- Personal Computer

Board settings
==============
1. Connect 3.3V to JP4-3 (VDDA_ANA) to provide the ADC reference voltage.
2. Connect a voltage source to J9-1 (PTA0 ADC0_A0) as the ADC analog input.

Note: JP4-3 is the VDDA_ANA supply pin which serves as the ADC reference voltage.
The actual VDDA_ANA voltage is slightly lower than 3.3V due to the voltage drop
across the onboard Schottky diode (RB160M-30).

Prepare the Demo
================
1. Connect a USB cable between the host PC and the LOC board J7.
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
LPADC Polling Example
ADC Full Range: 4096
Please press any key to get user channel's ADC value.
ADC value: 2057
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
