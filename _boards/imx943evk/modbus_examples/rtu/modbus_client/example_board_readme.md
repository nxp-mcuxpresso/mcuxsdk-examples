Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Use Dupont wire to connect the pin of the two MIMXRT1180-EVK boards
        J39-2(RX)----J39-4(TX)
        J39-4(TX)----J39-2(RX)
        J39-3(GND)---J39-3(GND)
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.
Note: The serial terminal of both boards must be opened.