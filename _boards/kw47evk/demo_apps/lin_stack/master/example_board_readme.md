Overview
========
The LIN (Local Interconnect Network) demo application is built on Low Power UART.
SW3 is used as button 1 to set master node to sleep and SW2 is used as button 2 to wakeup the master node.

Hardware requirements
=====================
- Two micro USB cables
- Two KW47-EVK boards
- Personal Computer
- Power adapter 12 V
- Three Dupont female-to-female wires

Board settings
==============
- Connect J11-1 of the two boards
- Connect J11-2 of the two boards
- Connect J11-4 of the two boards
- Connect 12 V adapter to J9
- Short the JP19-1 and JP19-2 with the jumper

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the boards.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6.  Download the program for master node to the target board.
