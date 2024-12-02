Hardware requirements
===================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer
- USB to Com converter

Board settings
============
J4-10 connect to TxD of the USB to Com converter.
J4-12 connect to RxD of the USB to Com converter.
J3-14 connect to GND of the USB to Com converter 

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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Uart functional API interrupt example
Board receives characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
