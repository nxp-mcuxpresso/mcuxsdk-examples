Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
==============
- LPADC CH6A Input signal J4-2(ptc11)

Prepare the Demo
================
1.  Connect a USB to Serial Line between the host PC and the J73 serial port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Polling Example
Please press any key to get user channel's ADC value.
ADC value: 4079
ADC value: 4079
ADC value: 4080
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
