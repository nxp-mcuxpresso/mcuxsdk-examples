Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer
- Oscilloscope

Board settings
==============

* Probe the pwm signal using an oscilloscope
 - At J102-15(PWM1_A0)
 - At J102-11(PWM1_A1)
 - At J102-7 (PWM1_A2)
 - At J102-13(PWM1_B0)

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~
