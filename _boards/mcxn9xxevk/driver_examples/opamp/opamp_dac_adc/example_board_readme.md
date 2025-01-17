  Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N9XX-EVK board
- Personal Computer

Board settings
Positive input channel0 J4-1(P4_12) and negative input channel J4_3(OPAMP0_INN) connect to GND.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
OPAMP DAC ADC EXAMPLE!

Please input a value (0 - 4095) for DAC:1500
Input DAC value is 1500
Current DAC output is about 1209.000 mV
Please press any key excluding key (R or r) to get user channel's ADC value.
Vsw1 ADC value: 1499
Actual voltage on Vsw1: 1.208V
Vref ADC value: 1498
Actual voltage on Vref: 1.207V


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vref stands for positive reference voltage output while Vsw1 stands for OPAMP output voltage. 
OPAMP output voltage ranges around 0 to VDDA-0.8V.