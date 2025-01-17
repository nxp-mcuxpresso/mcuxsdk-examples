Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
==============
If ENABLE_LOOPBACK is not defined, the example requires 2 sets of boards, each of them is mounted with the base board.
To make example work, connections needed to be as follows on each base board:

Board to board connections needed to be as follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Node A           connect to      Node B
Pin Name   Board Location         Pin Name    Board Location
CANH       J21-1                  CANH        J21-1
CANL       J21-2                  CANL        J21-2
GND        J21-4                  GND         J21-4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
