Hardware requirements
===================
- Mini/micro USB cable
- IMX943-EVK board
- USB to TTL tool
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Use Dupont wire to connect the USB to TTL tool to the pins of the board
        J48-2(RX)----USB to TTL tool(TX)
        J48-4(TX)----USB to TTL tool(RX)
        J47-14(GND)---USB to TTL tool(GND)
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.