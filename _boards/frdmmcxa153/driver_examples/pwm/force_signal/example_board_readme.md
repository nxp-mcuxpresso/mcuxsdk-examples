Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA153 board
- Personal Computer
- Oscilloscope

Board settings
==============
* Probe the pwm signal using an oscilloscope
 - PWM0_A0 output signal J3-15(PIO3_6).
 - PWM0_B0 output signal J3-13(PIO3_7).
 - GPIO output signal J1-14(PIO3_13)

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
*** eFlexPWM Force Signal Update Example ***
PWM0_A0 / PWM0_B0 complementary pair, 1000 Hz, 30% duty
Observe PWM0_A0 and PWM0_B0 on an oscilloscope.

Phase 0: Normal - DTSRCSEL=UsePwm, IPOL=PwmA (30% duty on A0)
Phase 1: DTSRCSEL update via FORCE_OUT -> InvertedPwm
         DTSRCSEL=InvertedPwm active (observe duty 70% on A0)
Phase 2: Atomic DTSRCSEL + IPOL update via single FORCE_OUT
         DTSRCSEL=UsePwm, IPOL=PwmB active (30% duty on A0)
Phase 3: Restore DTSRCSEL=UsePwm + IPOL=PwmA atomically
         Back to initial state (30% duty on A0)
~~~~~~~~~~~~~~~~~~~~~~~
