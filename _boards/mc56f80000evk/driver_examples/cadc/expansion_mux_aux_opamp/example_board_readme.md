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
Remove jumper for J20, J15, J16.
OPAMPA CH0 input signal J20-2(GPIOA1).The OPAMPA output signal (J1-10) is connected to the ANA4C (J1-12).

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
CADC expansion mux aux OPAMP example.
Press any key to trigger the conversion and toggle the OPAMP gain.
ADC Value: 3960
ADC Value: 7952
ADC Value: 15896
ADC Value: 31800
ADC Value: 3944

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
User can press any key to get converter data and toggle OPAMP gain.

The example initial OPAMP gain X1 , the OPAMP input voltage range is about 0-3.3v.

The first trigger ADC conversion, the OPAMP gain is switched to X1, the OPAMP input voltage range is about 0-3.3v.

The second trigger ADC conversion, the OPAMP gain is switched to X2, the OPAMP input voltage range is about 0-1.65v.

The third trigger ADC conversion, the OPAMP gain is switched to X4, the OPAMP input voltage range is about 0-0.82v.

The fourth trigger ADC conversion, the OPAMP gain is switched to X8, the OPAMP input voltage range is about 0-0.41v.
