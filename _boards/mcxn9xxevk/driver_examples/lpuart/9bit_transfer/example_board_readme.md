Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N9XX-EVK board
- Personal Computer
- USB to TTL converter

Board settings
============
Connect the USB to TTL converter to MCX-N9XX-EVK board.
MCX-N9XX-EVK UART pins:
TX pin is connected with RX pin.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPUART0    CONNECTS TO        LPUART0
Pin Name   Board Location     Pin Name  Board Location
TXD        J20-9(P0_12)       RXD       J20-12(P0_13)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the MCU-Link terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPUART 9-bit mode example begins
LPUART is configured with 9bit
LPUART will send data out:


0x100  0x101  0x102  0x103  0x104  0x105  0x106  0x107  
0x108  0x109  0x10a  0x10b  0x10c  0x10d  0x10e  0x10f  

LPUART received data:


0x100  0x101  0x102  0x103  0x104  0x105  0x106  0x107  
0x108  0x109  0x10a  0x10b  0x10c  0x10d  0x10e  0x10f  

All data matches!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~