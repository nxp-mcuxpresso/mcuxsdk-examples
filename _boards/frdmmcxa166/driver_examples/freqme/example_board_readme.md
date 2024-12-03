Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA166 board
- Personal Computer

Board settings
============
To measure the frequency or pulse width of external input clock, please connect input clock signal to
J1_7(P1_4, FREQME_CLK_IN0) or J2_19(P1_5, FREQME_CLK_IN1)


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
The log below shows the output of the freqme_interrupt demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FREQME Interrupt Example!
Please select operate mode...
        A -- Frequency Measurement Mode.
        B -- Pulse Width Measurement Mode.
Frequency Measurement Mode Selected!
Please select the target clock:
                A -- XTAL32MHz
                B -- FRO_OSC_12M
                C -- FREQME_GPIO_CLK_A
                D -- FREQME_GPIO_CLK_B
Please input the scale factor of reference clock(Ranges from 0 to 31).
20
Target clock frequency is 11999725 Hz.
Please select operate mode...
        A -- Frequency Measurement Mode.
        B -- Pulse Width Measurement Mode.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
