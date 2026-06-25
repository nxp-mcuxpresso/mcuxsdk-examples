Overview
========
The LIN (Local Interconnect Network) demo application is built on Low Power UART.
SW3 is used as button 1 to set master node to sleep and SW2 is used as button 2 to wakeup the master node.

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
- Connect 12V adapter to J1

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J28.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
