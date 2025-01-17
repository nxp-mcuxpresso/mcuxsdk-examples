Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC444 board
- Personal Computer

Board settings
==============
The DMA memory to memory demo does not call for any special hardware configurations.

Prepare the Demo
===============
1. Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2. Open a serial terminal with the following settings:
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
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
DMA memory to memory transfer example begin.

Destination Buffer:
0       0       0       0

DMA memory to memory transfer example finish.

Destination Buffer:
1       2       3       4
~~~~~~~~~~~~~~~~~~~~~

