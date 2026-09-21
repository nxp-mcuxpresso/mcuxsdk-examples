Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer
- Oscilloscope

Board settings
==============
* Probe the pwm signal using an oscilloscope
 - PWM0_A0 output signal J3-15(PIO2_23).
 - PWM0_B0 output signal J3-13(PIO2_22).
 - PWM0_A1 output signal J3-11(PIO2_21).
 - PWM0_A2 output signal J3-7(PIO2_19).
* Connet J3-8(LDO_3V3) to following pins to set PWM output in fault state
 - TRIG_IN4  input signal J4-1(PIO3_12).
 - TRIG_IN5  input signal J4-3(PIO3_13).
 - TRIG_IN6  input signal J4-5(PIO3_14).
 - TRIG_IN7  input signal J4-7(PIO3_15).

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
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
