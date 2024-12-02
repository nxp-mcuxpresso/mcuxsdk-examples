Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
============
connect input signal to J1-8

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
TPM dual-edge capture example

Once the input signal is received the input capture values are printed
The input signal's pulse width is calculated from the capture values & printed

Capture value C(n)V=11d5a52

Capture value C(n+1)V=11d5a60

Input signals pulse width=2 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
