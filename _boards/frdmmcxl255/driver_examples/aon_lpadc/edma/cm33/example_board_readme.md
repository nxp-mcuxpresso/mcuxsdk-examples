Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J16) on the target board.
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
The log below shows the output of the hello world demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpadc edma example
Press any key to get the ADC conversion result
10 times conversion result:
Value[0] = 36724
Value[1] = 36728
Value[2] = 36746
Value[3] = 36761
Value[4] = 36765
Value[5] = 36772
Value[6] = 36776
Value[7] = 36807
Value[8] = 36803
Value[9] = 36810
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
