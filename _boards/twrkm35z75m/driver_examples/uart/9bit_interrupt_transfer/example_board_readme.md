Hardware requirements
===================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer

Board settings
============
UART one board:
Using instance 0 of UART interface to transfer data to itself.
TX pin is connected with RX pin.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UART0      CONNECTS TO        UART0
Pin Name   Board Location     Pin Name  Board Location
TXD        J25-22             RXD       J25-24
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J27) on the target board.
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
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UART 9-bit mode example begins
UART is configured with address, only data sent to itself after matched address can be received
UART will send first piece of data out:

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  

UART will address itself
UART will send the other piece of data out:

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

UART received data:

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

All data matches!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
