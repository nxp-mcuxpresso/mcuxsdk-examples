Hardware requirements
=====================
- Personal Computer
- IMX943EVK board
- 12V~20V power supply
- J-Link Debug Probe
- Micro USB cable

Board settings
============
Set 0~3.3V voltage input to below ADC channels by using DC power supply
ADC channel 4(J9-2)
ADC channel 5(J9-5)
ADC channel 6(J9-8)

Connect J43-12 to oscilloscope to capture the trigger signal.

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect the 12V~20V power supply and J-Link Debug Probe to the board, and switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you will see similar information from the terminal shown below. You will also capture the trigger signal on OSC after ADC value get.

~~~~~~~~~~~~~~~~~~~~~
 IPSYNC trigger signal connected to output

 ADC polling example.
 Please press any key to get the conversion result.
 The channel 4 ADC conversion data is 4084.
 The channel 5 ADC conversion data is 3740.
 The channel 6 ADC conversion data is 2049.
 Please press any key to get the conversion result.
 The channel 4 ADC conversion data is 4085.
 The channel 5 ADC conversion data is 3741.
 The channel 6 ADC conversion data is 2049.
 Please press any key to get the conversion result.
 The channel 4 ADC conversion data is 4084.
 The channel 5 ADC conversion data is 3739.
 The channel 6 ADC conversion data is 2049.
~~~~~~~~~~~~~~~~~~~~~
