Overview
========
The LIN (Local Interconnect Network) demo application is a use-case of LIN stack driver built on Low Power UART. 
This application demonstrates a signal transfer between the two nodes. Master node and slave node signals. Both 
master and slave node must have implemented the timer that is used by timeout service routine every 500 us and 
master node use the same timer to check for switch of the frame table scheduler every 5 ms, but the real period between
frames may larger than 5ms according to the configurations.

User can press the SW3 button to stop ticks count, and press the SW2 button to restart the ticks count.

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

Running the demo
================
If communication is succesfull, press the SW3 button to increase the ticks count then you will see the LED red is ON.
Then press the SW2 button to reset the ticks count then you will see the LED blue is ON.
And slave node will print:
~~~~~~~~~~~~
LIN slave demo start!
Slave will send data to master and receive command from master.
Slave will control the LED according to the command from master.
 -> LED 2 is ON!
 -> LED 3 is ON!
 -> LED 2 is ON!
 -> LED 3 is ON!
 -> LED 2 is ON!

~~~~~~~~~~~~
