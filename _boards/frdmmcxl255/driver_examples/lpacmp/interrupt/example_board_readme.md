Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
1. Connect a external voltage source to the J3-3.
2. Connect J8-9(RX) and J8-11(TX) to a USB-TTL module, and then connect the module to the PC to observe the AON domain UART output.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpacmp interrupt example
Press any key to start the comparsion
LPACMP positive port voltage is higher than the negative port voltage
Press any key to start the comparsion
LPACMP positive port voltage is higher than the negative port voltage
Press any key to start the comparsion
LPACMP positive port voltage is higher than the negative port voltage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~