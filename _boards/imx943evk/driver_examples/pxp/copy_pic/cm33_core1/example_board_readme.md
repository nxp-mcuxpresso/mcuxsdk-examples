Hardware requirements
===================
- Micro USB cable
- IMX943-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- Headphone

Board settings
============
No special settings are required.

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW1(SYS_PWR) to power on the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
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
When the example runs, the screen shows what described in overview.
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PXP picture copy example start...
Copy from (0, 0) in input image to (10, 20) in output image, size: (16, 8)

PXP picture copy example success...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
