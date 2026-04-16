Hardware requirements
=====================
- Personal Computer
- IMX937LPD5BB-19-REV-A EVK board
- 12V~20V power supply
- J-Link Debug Probe
- Micro USB cable

Board settings
============
Connect J9-5 to J2-3

**Please note this application can't support running with Linux BSP!**

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
When the example runs successfully, you will see similar information from the terminal shown below.

~~~~~~~~~~~~~~~~~~~~~
 ADC selftest example.
 The channel 0 ADC conversion data is 4059.
 The channel 2 ADC conversion data is 1462.
 The channel 3 ADC conversion data is 691.
 The channel 0 ADC conversion data is 4060.
 The channel 2 ADC conversion data is 1463.
 The channel 3 ADC conversion data is 692.
 The channel 0 ADC conversion data is 4055.
 The channel 2 ADC conversion data is 1456.
 The channel 3 ADC conversion data is 697.
 The channel 0 ADC conversion data is 4058.
 The channel 2 ADC conversion data is 1461.
 The channel 3 ADC conversion data is 691.
 The channel 0 ADC conversion data is 4075.
 The channel 2 ADC conversion data is 1457.
 The channel 3 ADC conversion data is 695.
 The channel 0 ADC conversion data is 4050.
 The channel 2 ADC conversion data is 1461.
 The channel 3 ADC conversion data is 684.
 The channel 0 ADC conversion data is 4056.
 The channel 2 ADC conversion data is 1459.
 The channel 3 ADC conversion data is 694.
 Self-test error occurred!
 ADC self-test watchdog sequence error occurred.
~~~~~~~~~~~~~~~~~~~~~
