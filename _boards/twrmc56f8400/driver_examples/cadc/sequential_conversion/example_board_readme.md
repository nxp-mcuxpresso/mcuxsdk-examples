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
Connect J19 with jumper, 1-2, 3-4.
Connect J1 with jumper, 1-2, 3-4.
Connect J2 with jumper, 1-2, 3-4.

Note:
  The sample0(thermistor 4) works in fully differential mode and the differential pair is selected as ANA4 and ANA5.
  The sample1(thermistor 3) works in fully differential mode and the differential pair is selected as ANA6 and ANA7.
  The sample2(thermistor 1) works in fully differential mode and the differential pair is selected as ANB4 and ANB5.
  The sample3(thermistor 2) works in fully differential mode and the differential pair is selected as ANB6 and ANB7.

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

~~~~~~~~~~~~~~~~~~~~~~
CADC sequential conversion example.
Press any key to trigger the conversion ...

Sample 0        Sample 1        Sample 2        Sample 3

25728           18200           16992           25648
25728           18192           16992           25648

............
............
~~~~~~~~~~~~~~~~~~~~~

User can press any key to continue the converter and get more converter data. Touching thermistor
on board then the output values of sample will be updated.

