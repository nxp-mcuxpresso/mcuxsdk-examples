Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer

Board settings
==============
The example requires 2 sets of boards, each of them are mounted with the base board. Using a male to male CAN
cable to connect the CAN2 instance (J42) between the 2 base boards.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

Running the demo
================
After connecting the two boards, these instructions display on each terminal window.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B.

~~~~~~~~~~~~~~~~~~~~~
Consider special errata 005829 feature, the 1st valid MB should be used as reserved one.
The TX MB number will change from 8 to 9, while RX MB number will change from 9 to 10 
This message displays on the node A terminal:


******* FLEXCAN RTR Non-Blocking EXAMPLE *******
      Message format: Standard (11 bit id)
      Message buffer 9 used for Rx.
      Message buffer 10 used for Tx.
      Interrupt Mode: Enabled
************************************************

Please select local node as A or B:
Note: Node B should start first.
Node:A
Press any key to trigger one-shot remote request

Remote request message sent
Response message word0: 0x0, word1: 0x55
Response message Timestamp: 63932

Remote request message sent
Response message word0: 0x1, word1: 0x55
Response message Timestamp: 23889

Remote request message sent
Response message word0: 0x2, word1: 0x55
Response message Timestamp: 16638
~~~~~~~~~~~~~~~~~~~~~


~~~~~~~~~~~~~~~~~~~~~
This message displays on the node B terminal:

******* FLEXCAN RTR Non-Blocking EXAMPLE *******
      Message format: Standard (11 bit id)
      Message buffer 9 used for Rx.
      Message buffer 10 used for Tx.
      Interrupt Mode: Enabled
************************************************

Please select local node as A or B:
Note: Node B should start first.
Node:B
Start to wait request from Node A

Received remote request from Node A
Response message sent
Update next response message.

Received remote request from Node A
Response message sent
Update next response message.

Received remote request from Node A
Response message sent
Update next response message.
~~~~~~~~~~~~~~~~~~~~~

Note:
This case will be blocked when transferring data on armgcc debug/sdram_debug target. This issue has been fixed in later release by open ERR005829 feature.
