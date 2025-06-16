Hardware requirements
=====================
- Micro USB cable
- One IMX95LP4X-EVK boards
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Populate R742/743, don't populate R740/741 on SOM board

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
I3C master read sensor data example.
Temperature:32.812500
Temperature:32.812500
Temperature:32.812500
~~~~~~~~~~~~~~~~~~~~~
