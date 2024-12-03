Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- MCUXpresso10.1.0

Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer

Board settings
==============
CN3-20 is ACOMP negative input pin, which can sample external voltage.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPC_ACOMP Interrupt Example.
The example compares analog input to the voltage ladder output(ACOMP negative port).
The LED will be turned ON/OFF when the analog input is LOWER/HIGHER than the ladder's output.
Change the analog input voltage to see the LED status.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

