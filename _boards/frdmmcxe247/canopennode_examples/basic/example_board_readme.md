Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE247 Board
- Personal Computer

Board settings
==============
The example requires 2 boards.

Board to board connections needed to be as follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Node A           connect to      Node B
Pin Name   Board Location         Pin Name    Board Location
CANH       J17-1                  CANH        J17-1
CANL       J17-2                  CANL        J17-2
GND        J17-4                  GND         J17-4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect the type-C USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.
