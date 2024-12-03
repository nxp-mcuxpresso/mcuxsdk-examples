Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
1. Connect ADC channel1 (ADC_CH_01) to voltage supply which is less than 1.8v.
2. Get ADC conversion data three times.
3. Connect ADC channel1 (ADC_CH_01) to voltage supply which is more than 1.8v.
4. Get ADC conversion data one time.

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
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
 The channel 1 ADC conversion value is 3664.
 The channel 1 ADC conversion value is 3664.
 The channel 1 ADC conversion value is 3666.
 The channel 1 ADC conversion value is 4070.
 Current ADC value is higher than the high threshold.
~~~~~~~~~~~~~~~~~~~~~
