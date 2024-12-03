Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA166 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EDMA channel link example begin.

Destination Buffer 0:

0   0   0   0   

Destination Buffer 1:

0   0   0   0   

Destination Buffer 2:

0   0   0   0   

EDMA channel link example finish.

Destination Buffer 0:

1   2   3   4


Destination Buffer 1:

1   2   3   4   

Destination Buffer 2:

1   2   3   4   
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

