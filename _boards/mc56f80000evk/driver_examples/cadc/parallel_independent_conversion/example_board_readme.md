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
- Supply external voltage(0~3.3V) to ANA4(J4-4) and ANA5(J4-6).
- Supply external voltage(0~3.3V) to ANB4(J4-10) and ANB5(J4-12).
- Supply external voltage(0~3.3V) to ANB2(J2-9) and ANB3(J2-5).

Note:
  The sample0 works in fully differential mode and the differential pair is selected as ANA4 and ANA5.
  The sample1 works in fully differential mode and the differential pair is selected as ANA6 and ANA7.
  And the thermistor in MC56F80000-EVK board is connected to ANA6 and ANA7.
  The sample8 works in fully differential mode and the differential pair is selected as ANB4 and ANB5. 
  The sample9 works in fully differential mode and the differential pair is selected as ANB2 and ANB3.

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

~~~~~~~~~~~~~~~~~~~~~~~~
CADC parallel independent conversion example.
Press any key to trigger the conversion ...

Sample 0        Sample 1        Sample 8        Sample 9

16344           23760           28752           28304
16344           23760           28752           28352

................
................
~~~~~~~~~~~~~~~~~~~~~~~~
User can press any key to continue the converter and get more converter data. Touching thermistor
on board then the output values of sample will be updated.
