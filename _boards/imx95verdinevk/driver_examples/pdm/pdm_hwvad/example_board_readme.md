Hardware requirements
=====================
- Micro USB cable
- IMX95VERDIN-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Switch S4 to position2

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1.  Connect 12V~20V power supply and JLink Plus to the board, switch S6 to power on the board
2.  Connect a micro USB cable between the host PC and the J4 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, please speak or play a song nearby microphone,
then you will see log on the terminal like:
~~~~~~~~~~~~~~~~~~~
PDM hwvad example started!

Voice detected

Voice detected

...

PDM hwvad example finished!
~~~~~~~~~~~~~~~~~~~

Note: There is a variable in this example which initial as 50, once voice detected, it will decrease by one until 0, then example finish.
