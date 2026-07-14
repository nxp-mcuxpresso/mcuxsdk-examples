Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer
- Oscilloscope

Board settings
==============
* Probe the pwm signal using an oscilloscope
 - PWM0_A0 output signal J24-6(PTC0).
 - PWM0_A1 output signal J19-2(PTA18).
 - PWM0_A2 output signal J23-3(PTA20).
 - PWM0_B0 output signal J24-5(PIC1).
* Connet J19-8(MCU_3V3) to following pins to set PWM output in fault state
 - TRGMUX_IN1 input signal J24-1(PTA0).
 - TRGMUX_IN2 input signal J23-8(PTD3).

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

