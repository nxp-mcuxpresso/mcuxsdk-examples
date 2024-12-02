Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s16 board
- Personal Computer

Board settings
============
The jumper setting:
    Default jumpers configuration does not work,  you will need to add JP20 and JP21 (JP22 optional for ADC use)

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
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
The log below shows the output of the temperature measurement example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Temperature Measurement Example
ADC Full Range: 65536
Please press any key to get temperature from the internal temperature sensor.
Current temperature: 29.342
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
