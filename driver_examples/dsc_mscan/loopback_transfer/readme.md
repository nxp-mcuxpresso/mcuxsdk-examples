# dsc_mscan_loopback_transfer

## Overview
The mscan_loopback example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The example will config one MSCAN Message
Buffer to Rx Message Buffer and the other MSCAN Message Buffer to Tx Message Buffer with same ID.
After that, the example will send a CAN Message from the Tx Message Buffer to the Rx Message Buffer
through internal loopback interconnect and print out the Message payload to terminal.

## Supported Boards
