Hardware requirements
=====================
- Micro USB cable
- MIMX8MN-EVK board with MT25QU256ABA Quad NOR flash(SoM board SCH-38823 REV A1~REV A6)
- Personal Computer

Board settings
============
No special board settings are required.
The FlexSPI interface is directly connected to the on-board MT25QU256ABA
Quad NOR flash via NAND pads muxed to QSPI function.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the debug USB port on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXSPI example started!
Vendor ID: 0x20
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
