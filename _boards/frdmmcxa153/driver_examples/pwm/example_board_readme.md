Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA153 board
- Personal Computer
- Oscilloscope

Board settings
==============
* Probe the pwm signal using an oscilloscope
 - ADC PWM0_A0 output signal J3-15(PIO3_6).
 - ADC PWM0_B0 output signal J3-13(PIO3_7).
 - ADC PWM0_A1 output signal J3-11(PIO3_8).
 - ADC PWM0_A2 output signal J3-7(PIO3_10).
* Connet J3-8(LDO_3V3) to following pins to set PWM output in fault state
 - TRIG_IN0 input signal J1-8(PIO3_0).
 - TRIG_IN1 input signal J1-16(PIO3_1).
 - TRIG_IN2 input signal J1-3(PIO1_6).
 - TRIG_IN3 input signal J4-6(PIO1_13).

Prepare the Demo
================
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J15) on the target board.
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~
