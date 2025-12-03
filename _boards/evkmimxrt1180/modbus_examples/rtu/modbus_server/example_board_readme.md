Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- USB to TTL tool
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Use Dupont wire to connect the USB to TTL tool to the pins of the board

        J44-20(RX)----USB to TTL tool(TX)
        J44-18(TX)----USB to TTL tool(RX)
        J44-17(GND)---USB to TTL tool(GND)
        
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Press the reset button on your board to start the demo.