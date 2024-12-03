Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
============
connect input signal to J1-15

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TPM input capture example

Once the input signal is received the input capture value is printed

Capture value C(n)V=27da02b
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
