Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer
- Oscilloscope

Board settings
==============

* Probe the pwm signal using an oscilloscope
 - At J3-15(PWM1_A0)
 - At J3-11(PWM1_A1)
 - At J3-7 (PWM1_A2)
 - At J3-13(PWM1_B0)

Prepare the Demo
================
1. Connect a USB cable between the PC host and the MCU-Link USB port on the board.
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
