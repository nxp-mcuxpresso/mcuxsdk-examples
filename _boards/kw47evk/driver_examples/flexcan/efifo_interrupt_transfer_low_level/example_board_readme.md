Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============
Example requires 2 sets of boards, each of them is mounted with the base board.
To make example work, connections needed to be as follows on each base board:

Board to board connections needed to be as follows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Node A           connect to      Node B
Pin Name   Board Location         Pin Name    Board Location
CANH       J10-1                  CANH        J10-1
CANL       J10-2                  CANL        J10-2
GND        J10-4                  GND         J10-4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note: Plug the 12V power supply on J9 to supply the P5V_CAN.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
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

This message displays on the node A terminal:
~~~~~~~~~~~~~~~~~~~~~
FlexCAN Enhanced Rx FIFO low-level interrupt example.
Board to board mode.
Node B: Enhanced Rx FIFO used for Rx (frame count unknown in advance).
Node A: Message buffer 0 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:A
Please input the number of CAN FD messages to be send and end with enter.
8
Send Msg0 to Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, id = 0x123.
Send Msg1 to Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, id = 0x124.
Send Msg2 to Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, id = 0x125.
Send Msg3 to Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, id = 0x126.
Send Msg4 to Enhanced Rx FIFO: word0 = 0x4, word1 = 0x55, id = 0x123.
Send Msg5 to Enhanced Rx FIFO: word0 = 0x5, word1 = 0x55, id = 0x124.
Send Msg6 to Enhanced Rx FIFO: word0 = 0x6, word1 = 0x55, id = 0x125.
Send Msg7 to Enhanced Rx FIFO: word0 = 0x7, word1 = 0x55, id = 0x126.
Transmission done.

....
~~~~~~~~~~~~~~~~~~~~~

This message displays on the node B terminal:
~~~~~~~~~~~~~~~~~~~~~
FlexCAN Enhanced Rx FIFO low-level interrupt example.
Board to board mode.
Node B: Enhanced Rx FIFO used for Rx (frame count unknown in advance).
Node A: Message buffer 0 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:B
Waiting for frames from Node A...

Receive frame from Enhanced Rx FIFO: word0 = 0x0, word1 = 0x55, ID Filter Hit: 0, Time stamp: 1971.
Receive frame from Enhanced Rx FIFO: word0 = 0x1, word1 = 0x55, ID Filter Hit: 1, Time stamp: 11038.
Receive frame from Enhanced Rx FIFO: word0 = 0x2, word1 = 0x55, ID Filter Hit: 2, Time stamp: 20106.
Receive frame from Enhanced Rx FIFO: word0 = 0x3, word1 = 0x55, ID Filter Hit: 3, Time stamp: 29173.
Receive frame from Enhanced Rx FIFO: word0 = 0x4, word1 = 0x55, ID Filter Hit: 0, Time stamp: 38241.
Receive frame from Enhanced Rx FIFO: word0 = 0x5, word1 = 0x55, ID Filter Hit: 1, Time stamp: 47308.
Receive frame from Enhanced Rx FIFO: word0 = 0x6, word1 = 0x55, ID Filter Hit: 2, Time stamp: 56375.
Receive frame from Enhanced Rx FIFO: word0 = 0x7, word1 = 0x55, ID Filter Hit: 3, Time stamp: 65443.

....
~~~~~~~~~~~~~~~~~~~~~
