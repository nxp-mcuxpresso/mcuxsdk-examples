Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S69 board
- Personal Computer

Board settings
============
Connect SPI7 pins to SPI2 pins:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master(SPI7)		           Slave(SPI2)
Pin Name   Board Location     Pin Name   Board Location
MISO       P17  pin 12         MISO      P18  pin 6
MOSI       P17  pin 10         MOSI      P18  pin 10
SCK        P17  pin 14         SCK       P18  pin 8
SSEL1      P17  pin 1          SSEL0     P17  pin 16
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

