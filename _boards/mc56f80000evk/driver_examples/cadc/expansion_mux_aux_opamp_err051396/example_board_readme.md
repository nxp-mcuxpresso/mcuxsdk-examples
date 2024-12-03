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
Remove jumpers J20, J15, J16, J24, J25.
Connect 0.3V DC voltage to J20-2. (GPIOA1 for OPAMPA CH0 input signal)
Connect 0.3V DC voltage to J25-2. (GPIOB0 for OPAMPB CH3 input signal)
Connect J24-2 to ground like J3-14. (GPIOA0 for ADC ANA0 input signal, used for ADC timeslots for stabilize OPAMP)

Prepare the Demo
================
1.  Connect USB cable between the host PC and the USB(J12) port on the target baord to setup mulitlink debugger.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with PnE U-MultiLink debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
    Please be noted default FCF(flash configuration field) makes the MCU boot from flash.

Running the demo
================
Run the demo, and the user is requested to input choices for run with or without errata workaround.
    0-without workaround (failing to switch mux channel forward);
    1-with workaround (succeed to switch mux channel forward);
    Other inputs will be ignored.

If choose '0' without workaround, the message is like below.
The ADC value1 will not change much after 3 rounds.
  ADC Value: 3016
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 24040
  ADC Value1: 24056 --------- check if this value stay within small range or not
  ADC Value: 12000
  ADC Value1: 12040 --------- check if this value stay within small range or not
  ADC Value: 3016
  ADC Value1: 3064 --------- check if this value stay within small range or not
  ADC Value: 23992
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 11984
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 3008
  ADC Value1: 3048 --------- check if this value stay within small range or not
  ADC Value: 24008
  ADC Value1: 3048 --------- check if this value stay within small range or not
  ADC Value: 11984
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 3000
  ADC Value1: 3048 --------- check if this value stay within small range or not
  ADC Value: 24000
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 11992
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 3008
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 23984
  ADC Value1: 3048 --------- check if this value stay within small range or not
  ADC Value: 11976
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 3008
  ADC Value1: 3056 --------- check if this value stay within small range or not

If choose '1' with workaround, the message is like below.
The ADC value1 will change to different range for all rounds.
  ADC Value: 3008
  ADC Value1: 3048 --------- check if this value stay within small range or not
  ADC Value: 24064
  ADC Value1: 24080 --------- check if this value stay within small range or not
  ADC Value: 11984
  ADC Value1: 12040 --------- check if this value stay within small range or not
  ADC Value: 3016
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 23992
  ADC Value1: 24008 --------- check if this value stay within small range or not
  ADC Value: 11984
  ADC Value1: 12040 --------- check if this value stay within small range or not
  ADC Value: 3016
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 24024
  ADC Value1: 24040 --------- check if this value stay within small range or not
  ADC Value: 11992
  ADC Value1: 12032 --------- check if this value stay within small range or not
  ADC Value: 3016
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 24000
  ADC Value1: 24024 --------- check if this value stay within small range or not
  ADC Value: 12008
  ADC Value1: 12040 --------- check if this value stay within small range or not
  ADC Value: 3024
  ADC Value1: 3056 --------- check if this value stay within small range or not
  ADC Value: 23992
  ADC Value1: 24016 --------- check if this value stay within small range or not
  ADC Value: 11992
  ADC Value1: 12032 --------- check if this value stay within small range or not
  ADC Value: 3008
  ADC Value1: 3048 --------- check if this value stay within small range or not
