Hardware requirements
=====================
- Mini/micro USB cable
- K32W148-EVK Board
- Personal Computer

Board settings
============

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
SMSCM example start
The value of Security Counter Register (SCTR) is: 0xff
Write the value x to Plus Register: x = 3.
The value in security counter is plused to 0x102 successfully!
Write the value x to Minus Register: x = 2.
The value in security counter is minused to 0x100 successfully!
SMSCM example finished

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
