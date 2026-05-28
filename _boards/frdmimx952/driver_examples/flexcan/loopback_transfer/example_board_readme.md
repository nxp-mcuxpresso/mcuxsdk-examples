Hardware requirements
=====================
- Micro USB cable
- FRDM-IMX952 board
- JLink Plus
- 15V~20V USB Type-C PD power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 15V~20V USB Type-C PD power supply and JLink Plus to the board, switch SW1 to power on the board
2.  Connect a micro USB cable between the host PC and the J1 USB Type-C port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~

==FlexCAN loopback functional example -- Start.==


Send message from MB8 to MB9

tx word0 = 0x11223344

tx word1 = 0x55667788


Receved message from MB9

rx word0 = 0x11223344

rx word1 = 0x55667788


==FlexCAN loopback functional example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~
