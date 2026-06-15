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
 ADC watchdog example.
 Please press any key to get the conversion result.
 The channel 34 ADC conversion value is 4095.
 Current ADC value is higher than the high threshold.
~~~~~~~~~~~~~~~~~~~~~
