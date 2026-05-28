Hardware requirements
=====================
- Micro USB cable
- FRDM-IMX952 board
- JLink Plus
- 15V~20V USB Type-C PD power supply
- Personal Computer

Board settings
==============
No special settings are required.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1.  Connect 15V~20V USB Type-C PD power supply and JLink Plus to the board, switch SW1 to power on the board
2.  Connect a micro USB cable between the host PC and the J1 USB Type-C port on the target board.
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
When the demo runs successfully, you can hear the sound gathered from microphone via MQS output and the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~
PDM sai mqs interrupt example started!
~~~~~~~~~~~~~~~~~~~
