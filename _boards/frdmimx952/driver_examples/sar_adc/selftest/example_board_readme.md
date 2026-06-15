Hardware requirements
=====================
- Personal Computer
- IMX952LPD5BB-19-REV-A EVK board
- 12V~20V power supply
- J-Link Debug Probe
- Micro USB cable

Board settings
============
Populate a 100k resistor at R213, and a 200k resistor at R215

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
When the example runs successfully, you will see similar information from the terminal shown below.

~~~~~~~~~~~~~~~~~~~~~
 ADC selftest example.
 The channel 34 ADC conversion data is 4084.
 The channel 2 ADC conversion data is 2047.
 The channel 3 ADC conversion data is 1365.
 The channel 34 ADC conversion data is 4086.
 The channel 2 ADC conversion data is 2049.
 The channel 3 ADC conversion data is 1363.
 The channel 34 ADC conversion data is 4083.
 The channel 2 ADC conversion data is 2046.
 The channel 3 ADC conversion data is 1366.
 The channel 34 ADC conversion data is 4085.
 The channel 2 ADC conversion data is 2048.
 The channel 3 ADC conversion data is 1364.
 The channel 34 ADC conversion data is 4084.
 The channel 2 ADC conversion data is 2047.
 The channel 3 ADC conversion data is 1365.
 Self-test error occurred!
 ADC self-test watchdog sequence error occurred.
~~~~~~~~~~~~~~~~~~~~~
