Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board, FRDM-TOUCH board
- Personal Computer

Board settings
============
Connect the FRDM-TOUCH board with FRDM-KE16Z.

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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running successfully, LED will be turned on when the electrode on FRDM-TOUCH board is touched
and the LED will be turned off after the electrode is released. 
The log output in terminal shall be similar as below:
~~~~~~~~~~~~~~~~~~~~~
TSI mutual mode demo.
TSI key touched.
TSI key released.
TSI key touched.
TSI key released.
~~~~~~~~~~~~~~~~~~~~~