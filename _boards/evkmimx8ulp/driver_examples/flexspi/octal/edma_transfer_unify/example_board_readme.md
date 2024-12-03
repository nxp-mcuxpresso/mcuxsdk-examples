Hardware requirements
=====================
- Mini/micro USB cable
- MIMX8ULP-EVK/EVK9 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  After power on board, stop the uboot and ensure the board will not autoboot to linux. 
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FLEXSPI edma example started!
JEDEC id bytes: c8, 68, 19
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
