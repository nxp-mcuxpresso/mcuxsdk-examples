Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
To measure the frequency or pulse width of external input clock, please connect input clock signal to:
FREQME_CLK_IN0 - P2_0 (J2)
FREQME_CLK_IN1 - P2_1 (J2)

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FREQME Interrupt Example!
Please select operate mode...
        A -- Frequency Measurement Mode.
        B -- Pulse Width Measurement Mode.
Frequency Measurement Mode Selected!
Please select the target clock:
                A -- CLK_IN
                B -- FRO_12M
                C -- FREQME_CLK_IN0
                D -- FREQME_CLK_IN1
Target clock: B
Please input the scale factor of reference clock(Ranges from 0 to 31).
20
Target clock frequency is 11999725 Hz.
Please select operate mode...
        A -- Frequency Measurement Mode.
        B -- Pulse Width Measurement Mode.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~