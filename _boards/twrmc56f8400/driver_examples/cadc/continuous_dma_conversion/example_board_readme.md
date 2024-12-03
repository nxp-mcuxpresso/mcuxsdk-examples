Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8400 board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
2. UART setting:
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
3. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
Connect J23 with jumper, 1-2, 3-4.

Note:
  The sample0(thermistor 4) works in fully differential mode and the differential pair is selected as ANA4 and ANA5.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~
CADC continuous dma conversion.
Press any key to trigger the conversion ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CADC continuous dma conversion.
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

