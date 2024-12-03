Hardware requirements
=====================
- Micro USB cable
- two IMX95LPD5-EVK board
- JLink Plus
- 12V~20V power supply
- Personal Computer

Board settings
==============
The example requires connecting between CAN pins of two boards.
The connection should be set as follows:

Between two boards:
- J17-2(CANH) node A, J17-2(CANH) node B
- J17-3(CANL) node A, J17-3(CANL) node B
- J17-4(GND) node A, J7-4(GND) node B

Prepare the Demo
================
1.  Connect 12V~20V power supply and JLink Plus to the board, switch SW4 to power on the board
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
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
When setting ENABLE_LOOPBACK macro and the example runs successfully, you can see the similar information
from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
FlexCAN Enhanced Rx FIFO edma example.
Loopback mode, Message buffer 0 used for Tx, Enhanced Rx FIFO used for Rx.

Press any key to trigger 4 transmission.

Send Msg0 to Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, id = 0x123.
Send Msg1 to Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, id = 0x124.
Send Msg2 to Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, id = 0x125.
Send Msg3 to Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, id = 0x126.

Receive Msg0 from Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, ID Filter Hit: 0, Time stamp: 41410.
Receive Msg1 from Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, ID Filter Hit: 1, Time stamp: 48882.
Receive Msg2 from Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, ID Filter Hit: 2, Time stamp: 56352.
Receive Msg3 from Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, ID Filter Hit: 3, Time stamp: 63821.

Press any key to trigger 4 transmission.
...
~~~~~~~~~~~~~~~~~~~~~

==FlexCAN loopback functional example -- Start.==

2 boards are required to be connected through CAN bus if ENABLE_LOOPBACK is not defined.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B.

This message displays on the node A terminal:
~~~~~~~~~~~~~~~~~~~~~
FlexCAN Enhanced Rx FIFO edma example.
Board to board mode.
Node B Enhanced Rx FIFO used for Rx.
Node A Message buffer 0 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:a
Press any key to trigger 4 transmission.

Send Msg0 to Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, id = 0x123.
Send Msg1 to Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, id = 0x124.
Send Msg2 to Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, id = 0x125.
Send Msg3 to Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, id = 0x126.

Press any key to trigger 4 transmission.

....
~~~~~~~~~~~~~~~~~~~~~

This message displays on the node B terminal:
~~~~~~~~~~~~~~~~~~~~~
FlexCAN Enhanced Rx FIFO interrupt example.
Board to board mode.
Node B Enhanced Rx FIFO used for Rx.
Node A Message buffer 0 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:b
Start to Wait data from Node A.

Receive Msg0 from Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, ID Filter Hit: 0, Time stamp: 24286.
Receive Msg1 from Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, ID Filter Hit: 1, Time stamp: 25059.
Receive Msg2 from Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, ID Filter Hit: 2, Time stamp: 25745.
Receive Msg3 from Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, ID Filter Hit: 3, Time stamp: 26431.

Wait for the next 4 messages!

....
~~~~~~~~~~~~~~~~~~~~~
