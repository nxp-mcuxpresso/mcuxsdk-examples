Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXE31B Board
- Personal Computer

Board settings
==============
This example uses two FlexCAN instances (FlexCAN0 and FlexCAN1) on a single board for internal communication.
The two CAN instances must be connected externally:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       FlexCAN0         connect to      FlexCAN1
Pin Name   Board Location         Pin Name    Board Location
CANH       J17-1                  CANH        J18-1
CANL       J17-2                  CANL        J18-2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: Verify the correct FlexCAN0 and FlexCAN1 pin locations on FRDM-MCXE31B board schematic.

Prepare the Demo
===============
1.  Connect the CAN buses between FlexCAN0 and FlexCAN1 as shown above.
2.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board. 
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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
********* FLEXCAN Interrupt (Low-Level) EXAMPLE *********
    Message format: Standard (11 bit id)
    FlexCAN_RX: Receiver (Message buffer 1)
    FlexCAN_TX: Transmitter (Message buffer 0)
    Interrupt Mode: Enabled (Low-Level API)
    Operation Mode: One-way communication
**********************************************************

Initializing FlexCAN_RX as receiver...
Initializing FlexCAN_TX as transmitter...

Press any key to trigger transmission from FlexCAN_TX to FlexCAN_RX

FlexCAN_TX: Transmitted frame with data 0x 0
FlexCAN_RX: Received frame - ID: 0x123, Data: 0x 0, Timestamp: 59456
Press any key to trigger the next transmission!

FlexCAN_TX: Transmitted frame with data 0x 1
FlexCAN_RX: Received frame - ID: 0x123, Data: 0x 1, Timestamp: 1339
Press any key to trigger the next transmission!

FlexCAN_TX: Transmitted frame with data 0x 2
FlexCAN_RX: Received frame - ID: 0x123, Data: 0x 2, Timestamp: 15741
Press any key to trigger the next transmission!

~~~~~~~~~~~~~~~~~~~~~
