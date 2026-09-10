Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer
- Oscilloscope

Board settings
============
- Connect an external signal source to P1_0 (pin 9, QTMR0_CH0) to provide the input capture signal.
- Use an oscilloscope to measure the PWM output signal on P1_1 (pin 10, QTMR0_CH1).

Prepare the Demo
===============
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
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

****Input capture example.****

****Provide a signal input to the QTMR pin****

Captured Period time=... us

****Output PWM example.****

*********Make sure to connect an oscilloscope.*********

****A 50% duty cycle PWM wave is observed on an oscilloscope.****

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
