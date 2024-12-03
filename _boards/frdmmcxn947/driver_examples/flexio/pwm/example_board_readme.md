Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 Board
- Personal Computer

Board settings
============
PWM output: J8-11(P0_8, FLEXIO_D0)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J17.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  Use oscilloscope to measure the output 100KHz PWM signal pin at J8-11 pin.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXIO_PWM demo start.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
