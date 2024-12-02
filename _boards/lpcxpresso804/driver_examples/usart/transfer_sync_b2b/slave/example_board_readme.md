Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso804 board
- Personal Computer

Board settings
============
Please connect the USART1_CLK pin to USART1_CLK pin directly.
No need to connect other pins. The data pins are shared.

    Master - USART1                Slave - USART1   
Pin Name   Board Location      Pin Name   Board Location            
TXD       CN3 pin 10           RXD       CN3 pin 9                   
RXD       CN3 pin 9            TXD       CN3 pin 10               
SCK       CN8 pin 2            SCK       CN8 pin 2 

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USART synchronous transfer example.
Slave is workinng...
All data matched, data transfer success!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
