Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- Personal Computer

Board settings
============
Make sure the FXOS8700CQ(U12) on the board is installed to run this demo.
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button to begin running the demo.

Running the demo
================
Disconnect any debugger to run this demo in case the sequence of entering sleep mode is interrupted.
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~
LPI2C_VLPS demo start...
X:    2,  Y: -148,  Z: 2107
X:   -2,  Y:  -97,  Z: 2086
~~~~~~~~~~~~
