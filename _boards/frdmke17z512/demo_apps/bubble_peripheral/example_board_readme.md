Hardware requirements
===================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer

Board settings
============
Make sure the FXLS8974CFR3(U12) on the board is installed to run this demo.
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar
information from the terminal as shown below.
~~~~~~~~~~~~~~~~~~~~~~~~~
Welcome to the BUBBLE example

You will see the change of angle data and LED brightness when change the angles of board

x=  6 y = 22
x=  8 y = 26
x= 10 y = 28
x= 10 y = 28
x= 11 y = 29
x= 11 y = 29
~~~~~~~~~~~~~~~~~~~~~~~~~
