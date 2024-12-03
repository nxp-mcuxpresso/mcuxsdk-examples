Hardware requirements
=====================
- 2 x Micro USB cable
- MC56F81000-EVK board
- Personal Computer
- Oscilloscope

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
- Connect J1-8 to J3-8(VDD) with a 10K resistor. Caution: the 10K resistor must be used in series within J1-8 and J3-8.
- Probe the pwm signal using an oscilloscope
   - At J3-15
   - At J3-13
   - At J3-11
   - At J3-7

  All PWM signals have the same frequency.
  The J3-13/15 PWM signal is a pair of duty cycle constantly changing complementary signals with a fixed dead time.
  The J3-11 duty cycle is half of J3-15, and J3-7 duty cycle is half of J3-11.

- Connect J1-8 to GND with a 10K resistor. Caution: the 10K resistor must be used in series within J1-8 and GND.
  Then J3-15/J3-11/J3-7 PWM signals will be disabled.

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
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~
