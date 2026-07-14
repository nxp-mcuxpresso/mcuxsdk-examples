Hardware requirements
=====================
- Mini/micro USB cable
- HVP-MCXA346 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the demo
================
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: The debug UART uses LPUART2 (P2_2/TX, P2_3/RX) connected to the on-board MCU-Link
debug circuit. Refer to the board schematic (SPF-96386) for connector details.
