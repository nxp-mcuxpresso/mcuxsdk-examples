Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE318 board
- Personal Computer

Board settings
==============

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
BCTU SoftWare Trigger Multiple Parallel Conversions Example.
Please press any key to get the ADC value
ADC_0 channel 48 value: 12005
ADC_1 channel 54 value: 4
ADC_2 channel 55 value: 32750
ADC_0 channel 48 value: 12002
ADC_1 channel 54 value: 4
ADC_2 channel 55 value: 32750
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
