Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

**Please note this application can't support running with Linux BSP!**


Prepare the Demo
================
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
TRNG Peripheral Driver Example
Generate 10 random numbers:
Random[0] = 0xE4C973F5
Random[1] = 0x25BEBC2B
Random[2] = 0x1A889794
Random[3] = 0xF723958
Random[4] = 0xD9818CFE
Random[5] = 0x409950E7
Random[6] = 0xA9315CA1
Random[7] = 0x5060CAC3
Random[8] = 0x93870888
Random[9] = 0x32DAB69B

 Press any key to continue...
~~~~~~~~~~~~~~~~~~~~~
