Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
==============
To make the example work, connections needed to be as follows:
Note: 
Jumper setting:
LPSPI MOSI(J2_8) connect to SINC MBIT0(J8_18)

Other jumpers keep default configuration.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal on PC for MCU-Link serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SINC LPSPI Example.
LPSPI default output value: 0x7f

Press any key to trigger sinc conversion!

SINC Result:0x7F


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

