Overview
========
The LIN (Local Interconnect Network) demo application is built on Low Power UART.
SW3 is used as button 1 to stop ticks count and SW2 is used as button 2 to restart the ticks count.

Hardware requirements
=====================
- Two micro USB cables
- Two KW45B41Z-EVK boards
- Personal Computer
- Power adapter 12 V
- Three Dupont female-to-female wires

Board settings
==============
- Connect J11-1 of the two boards
- Connect J11-2 of the two boards
- Connect J11-4 of the two boards
- Short the JP19-1 and JP19-2 with the jumper
- Short the J1-1 and J2-2 with the jumper due to the autobaudrate feature is enabled by default

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
