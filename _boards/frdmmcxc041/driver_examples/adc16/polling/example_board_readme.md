Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXC041 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect external signal in J4-2
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~
ADC16 polling Example.
ADC16_DoAutoCalibration() Done.
Press any key to get user channel's ADC value ...
ADC Value: 1985
ADC Value: 1689
ADC Value: 1355
ADC Value: 1625
ADC Value: 1575
ADC Value: 1471
~~~~~~~~~~~~~~~~~~~~~~~~
