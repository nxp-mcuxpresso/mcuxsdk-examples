Hardware requirements
=====================
- 2 x Micro USB cable
- MC56F81000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J5 pin1 and pin2
   > Connect J6 pin1 and pin2
2. Onboard PnE U-MultiLink debugger setting:
   > Connet J11 pin2 and pin3
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger PnE U-MultiLink.
     Note: when use external debugger(connected to J10), leave all J13 pins open.

Board settings
==============
- Supply external voltage(0~3.3V) to ANA4(J4-4) and ANA5(J4-6).
- Supply external voltage(0~3.3V) to ANB4(J4-10) and ANB5(J4-12).
- Supply external voltage(0~3.3V) to ANB2(J2-9) and ANB3(J2-5).

Note:
  The sample0 works in fully differential mode and the differential pair is selected as ANA4 and ANA5.
  The sample1 works in fully differential mode and the differential pair is selected as ANA6 and ANA7.
  And the thermistor in MC56F81000-EVK board is connected to ANA6 and ANA7.
  The sample2 works in fully differential mode and the differential pair is selected as ANB4 and ANB5. 
  The sample3 works in fully differential mode and the differential pair is selected as ANB2 and ANB3.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the USB(J26) port on the target board to setup USB to UART bridge.
2.  Connect USB cable between the host PC and the USB(J12) port on the target baord to setup mulitlink debugger.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board with PnE U-MultiLink debug configuration.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
    Please be noted default FCF(flash configuration field) makes the MCU boot from flash.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~
CADC sequential conversion example.
Press any key to trigger the conversion ...

Sample 0        Sample 1        Sample 2        Sample 3

16360           23848           28720           28304
16352           23848           28720           28304
16360           23856           28720           28304
16360           23912           28720           28304

............
............
~~~~~~~~~~~~~~~~~~~~~

User can press any key to continue the converter and get more converter data. Touching thermistor
on board then the output values of sample will be updated.

