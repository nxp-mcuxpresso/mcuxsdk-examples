Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
Connect the J4-8 to a voltage source.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
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
This demo will measure the Bandgap via ADC0 channel 48, measure VREFH via ADC0 channel 55, measure user defined
voltage via channel 5. In this demo, the user defined voltage is 1.67v.

When the example runs successfully, you will see similar information from the terminal shown below.
~~~~~~~~~~~~~~~~~~~~~
 ADC interrupt example.
 Please press any key to get the conversion result.
 The channel 48 ADC conversion data is 12031.
 The channel 55 ADC conversion data is 32765.
 The channel 5 ADC conversion data is 16698.
~~~~~~~~~~~~~~~~~~~~~
