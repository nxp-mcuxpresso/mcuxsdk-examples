Overview
========
The LIN (Local Interconnect Network) demo application is built on Low Power UART.
SW3 is used as button 1 to stop ticks count and SW2 is used as button 2 to restart the ticks count.

Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer
- Power adapter 12V
- Three Dupont female-to-female wires

Board settings
==============
- Connect J18-1 of the two boards
- Connect J18-2 of the two boards
- Connect J18-4 of the two boards
- Short the JP18-1(LPUART0_RX) and J19-3(PTB0_H) with the jumper due to the autobaudrate feature is enabled by default
- U30 and U12C affects the PTB0_H signal, causing it to remain at a low level.
  Note: Short JP6 to disable U30 before running this example.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the boards.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6.  Download the program for slave node to the target board.
