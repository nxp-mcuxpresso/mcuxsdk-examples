Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
Ensure R520 is removed, R518 is installed to make SW4 work.

No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board
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
When the example runs successfully, the following message is displayed in the terminal:
If you turn on the SW4 , and "SW4 is turned on" is shown on the terminal window

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example.

 SW4 is turned on.
 ......
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
