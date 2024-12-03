Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW10 to power on the board
2.  Connect a micro USB cable between the host PC and the J17 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

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
