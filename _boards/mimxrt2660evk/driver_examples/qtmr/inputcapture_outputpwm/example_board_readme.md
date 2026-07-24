Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer
- Oscilloscope

Board settings
==============
1. Input capture: connect PWM signal (0-1.8V, 1 kHz) to J94-13 (PIO3_0).
   Signal path: PIO3_0 (GPIO1_GPIO0) -> GPIO1_TrigOut0 -> XBAR2 -> XBAR0 -> QTMR0_IN0.

2. Output PWM: use oscilloscope to monitor J94-14 (PIO3_1).
   Signal path: QTMR0_OUT1 -> XBAR2 -> XBAR0 -> PIO3_1 (XBAR0_INOUT01, mux=A).

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

****Input capture example.****

****Provide a signal input to the QTMR pin****

Captured Period time=1000 us

****Output PWM example.****

*********Make sure to connect an oscilloscope.*********

****A 50 duty cycle PWM wave is observed on an oscilloscope.****

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
