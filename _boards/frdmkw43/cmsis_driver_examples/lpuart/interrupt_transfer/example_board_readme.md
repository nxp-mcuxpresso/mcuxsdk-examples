Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart CMSIS interrupt example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When you input 8 characters, system will echo it by LPUART and them would be seen on the OpenSDA terminal.
