Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer
- USB to Com Converter

Board settings
============
Change Jumper R112 from 1-2 to 1-3, Jumper R334 from 1-2 to 1-3.
Connect pin:
- RX of USB2COM to J61-5
- TX of USB2COM to J61-6
- GND of USB2COM to J61-49

Prepare the Demo
===============
1.  Connect the USB2COM Converter to the host PC
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
When the demo runs successfully, the log would be seen on the UART Terminal port which connected to the USB2COM like:

~~~~~~~~~~~~~~~~~~~~~
Flexio uart edma example
Board receives 8 characters then sends them out
Now please input:
~~~~~~~~~~~~~~~~~~~~~
