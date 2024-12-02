Hardware requirements
===================
- Micro USB cable
- Two LPCXpresso55S06 boards
- Personal Computer

Board settings
============
Connect SPI master on board to SPI slave on other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI3)           Slave_board(SPI3)                          
Pin Name   Board Location     Pin Name   Board Location                     
MISO       J12   pin 2        MISO       J12   pin 2
MOSI       J12   pin 1        MOSI       J12   pin 1
SCK        J10   pin 16       SCK        J10   pin 16
SSEL0      J10   pin 14       SSEL0      J10   pin 14
GND        J9    pin 8        GND        J9    pin 8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the CMSIS DAP terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Master Start...!

Succeed!
​~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~