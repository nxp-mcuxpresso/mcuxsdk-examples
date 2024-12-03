Hardware requirements
===================
- Micro USB cable
- IMX95VERDIN-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- Headphone

Board settings
============
Switch S3 to position1

Prepare the Demo
===============

Note: single boot with Linux BSP is required. SAI3 and EDMA4 are modules of WAKEUPMIX, A core and Linux
      BSP is responsible to configure proper permissions with TRDC.

1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch S6 to power on the board.
2.  Connect a micro USB cable between the host PC and the J4 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example

Running the demo
===============
When the demo runs successfully, you can hear the tone and the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI EDMA example finished!
 ~~~~~~~~~~~~~~~~~~~
