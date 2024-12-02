Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso845MAX board
- Personal Computer

Board settings
============
Please connect the USART1_CLK pin to USART2_CLK pin directly.
No need to connect other pins. The data pins are shared.

P0_24(J2-8) -> P0_25(J2-7)

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
Usart synchronous transfer example.
Master USART will send data to slave on one board.
All data matched, data transfer success!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
