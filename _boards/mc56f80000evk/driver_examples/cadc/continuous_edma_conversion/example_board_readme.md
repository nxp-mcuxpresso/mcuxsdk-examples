Hardware requirements
=====================
- Micro USB cable
- MC56F80000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J6 pin1 and pin2
   > Connect J5 pin1 and pin2
2. Reset pin setting:
   > Connect J11 pin2 and pin3
3. Debugger setting:
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8

Board settings
==============
No special settings are required.

Note:
  The sample0 works in fully differential mode and the differential pair is selected as ANA6 and ANA7.
  And the thermistor in MC56F80000-EVK board is connected to ANA6 and ANA7.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the J12 USB port on the target board. It setups Multilink and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with Multilink debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CADC continuous edma conversion.
Press any key to get user channel's ADC value ...
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122
ADC value: 3122

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

User can press any key to get more converter data. The ADC input channel is connected with thermistor,
so touching thermistor on board will get the ADC value updated.
