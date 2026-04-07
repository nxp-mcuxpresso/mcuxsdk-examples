Hardware requirements
=====================
- (2x) USB Type-C cable
- FRDM-IMX95 board
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect 12V~20V power supply to the board, switch SW4 to power on the board.
2.  Connect a USB-C cable between the host PC and the J31 USB port on the target board.
3.  Connect a USB-C cable between the host PC and the J8 USB port on the target board.
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Download the program to the target board.
6.  Re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, will get the similar messages on the terminal:
~~~~~~~~~~~~~~~~~~~~~~
Width: 1920, Height: 1088, Testing Loops: 16
---------------- g2d_open/close stress test ----------
---------------- g2d_alloc stress test ---------------
---------------- test dst YUV feature ----------------
RGBA to YUY2 time 2478us, 403fps, 843Mpixel/s ........
---------------- g2d blit performance ----------------
RGBA->RGBA time 2475us, 404fps, 844Mpixel/s ........
. . .
~~~~~~~~~~~~~~~~~~~~~~
