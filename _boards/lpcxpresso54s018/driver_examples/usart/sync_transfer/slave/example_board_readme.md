Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54s018 board
- Personal Computer

Board settings
============
Please connect pins as below shows:
    Master board            Slave board
	J9-8  (U9_RXD)  -----    J13-11(U9_TXD)
	J13-11(U9_TXD)  -----    J9-8  (U9_RXD)
	J9-9  (U9_SCLK) -----    J9-9  (U9_SCLK)
	J9-7  (GND)     -----    J9-7  (GND)
Note that, please start slave board first.


Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J8) on the target board.
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
This is USART synchronous transfer slave example.
Slave will receive data from master firstly, and then send back the received characters.
Please connect the pins as below shows:
   Master Board          Slave Board
   USART_TX      -------   USART_RX
   USART_RX      -------   USART_TX
   USART_SCLK    -------   USART_SCLK
   GND           -------   GND
Data receive complete. The data received from master are:

 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

Data send to master complete!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~