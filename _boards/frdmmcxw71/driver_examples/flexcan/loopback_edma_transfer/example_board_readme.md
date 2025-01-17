Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
2. Open a serial terminal on PC for the serial device with these settings:
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
When the example runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~
==FlexCAN loopback edma example -- Start.==

Send Msg1 to Rx FIFO: word0 = 0x11111111, word1 = 0x11111111.
Send Msg2 to Rx FIFO: word0 = 0x22222222, word1 = 0x22222222.
Send Msg3 to Rx FIFO: word0 = 0x33333333, word1 = 0x33333333.
Send Msg4 to Rx FIFO: word0 = 0x44444444, word1 = 0x44444444.

Receive Msg1 from FIFO: word0 = 0x11111111, word1 = 0x11111111, ID Filter Hit2.
Receive Msg2 from FIFO: word0 = 0x22222222, word1 = 0x22222222, ID Filter Hit2.
Receive Msg3 from FIFO: word0 = 0x33333333, word1 = 0x33333333, ID Filter Hit2.
Receive Msg4 from FIFO: word0 = 0x44444444, word1 = 0x44444444, ID Filter Hit2.

==FlexCAN loopback EDMA example -- Finish.==
~~~~~~~~~~~~~~~~~~~~~