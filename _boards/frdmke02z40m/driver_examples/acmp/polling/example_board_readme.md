Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE02Z40M board
- Personal Computer

Board settings
============
J2-2 is ACMP input pin.

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).
The LED will be turned ON/OFF when the analog input is LOWER/HIGHER than the DAC output.
Change the analog input voltage to see the LED status.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
User can change J2-2 pin to see the LED status.
