Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
============
Jumping caps are connected to JP15 and JP44 respectively.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2.  Connect a micro USB cable between the PC host and the USB-UART port (J21) on the board.
3.  The project set the 115200 baud rate as default, set flow as default. Use the USB-UART port (J21) to communicate.
    if need use 3000000 baud rate, need modify the code.
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

