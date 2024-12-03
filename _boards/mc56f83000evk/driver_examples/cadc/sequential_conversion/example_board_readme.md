Hardware requirements
=====================
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J19 pin2 and pin3
   > Connect J12 pin1 and pin2
   > Connect J13 pin1 and pin2
   > Connect J15 pin1 and pin2
   > Connect J16 pin2 and pin3
   > Connect J17 pin1 and pin2
2. Onboard OSJTAG debugger setting:
   > Leave J5 open
   > Leave J6 open
     Note: J6 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J11 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J10), leave all J11 pins open.

Board settings
==============
- Supply external voltage(0~3.3V) to ANA4(J4-4) and ANA5(J4-6).
- Supply external voltage(0~3.3V) to ANB0(J2-3) and ANB1(J4-8).
- Supply external voltage(0~3.3V) to ANB2(J2-9) and ANB3(J2-5).

Note:
  The sample0 works in fully differential mode and the differential pair is selected as ANA4 and ANA5.
  The sample1 works in fully differential mode and the differential pair is selected as ANA6 and ANA7.
  And the thermistor in MC56F83000-EVK board is connected to ANA6 and ANA7.
  The sample2 works in fully differential mode and the differential pair is selected as ANB0 and ANB1.
  The sample3 works in fully differential mode and the differential pair is selected as ANB2 and ANB3.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.

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

