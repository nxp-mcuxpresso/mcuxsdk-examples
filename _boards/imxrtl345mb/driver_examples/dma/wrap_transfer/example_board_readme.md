Hardware requirements
=====================
- Type-C USB cable
- IMXRTL345MB board
- Personal Computer

Board settings
==============

Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~
DMA wrap transfer example begin.

Destination address wrap transfer:
5	6	7	8	0	0	0	0
Source address wrap transfer:
1	2	3	4	1	2	3	4

DMA wrap transfer example finish.

~~~~~~~~~~~~~~~~~~~~~
