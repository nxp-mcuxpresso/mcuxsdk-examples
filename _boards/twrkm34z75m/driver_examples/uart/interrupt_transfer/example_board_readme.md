Hardware requirements
===================
- Mini USB cable
- TWR-KM34Z75M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

Uart interrupt example
Board receives 8 characters then sends them out
Now please input:

When you input 8 characters, system will echo it by UART and them would be seen on the OpenSDA terminal.

