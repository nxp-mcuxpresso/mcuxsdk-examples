Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE318 board
- Personal Computer

Board settings
==============
Connect the J3-11 to a oscilloscope to observe the 10KHz, 70% dutycycle PWM.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
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
The following lines are printed to the serial terminal when the demo program is executed.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BCTU HardWare Trigger Example
Please press any key to get the ADC value
ADC channel 48 value: 12023
ADC channel 48 value: 12022
ADC channel 48 value: 12024
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
