Hardware requirements
=====================
- Personal Computer
- IMX95LPD5BB-19-REV-A EVK board
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
 ADC polling example.
 Please press any key to get the conversion result.
 The channel 0 ADC conversion data is 4063.
 The channel 2 ADC conversion data is 1379.
 The channel 3 ADC conversion data is 682.
~~~~~~~~~~~~~~~~~~~~~
