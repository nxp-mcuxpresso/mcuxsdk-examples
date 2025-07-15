Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA346 board
- Personal Computer

Board settings
============
Connect PmodKYPD J1 to the FRDM board PMODE interface (J7).

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SmartDma Keyscan Example.
Button 1 is pressed
Button 2 is pressed
Button 3 is pressed
Button 4 is pressed
Button 5 is pressed
Button 6 is pressed
Button 7 is pressed
Button 8 is pressed
Button 9 is pressed
Button A is pressed
Button B is pressed
Button C is pressed
Button D is pressed
Button E is pressed
Button F is pressed
Button 0 is pressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
