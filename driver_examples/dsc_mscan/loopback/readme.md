# dsc_mscan_loopback

## Overview
The mscan_loopback_functional example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The example will configure one MSCAN Message Tx
buffer and Rx buffer with same ID.
After that, the example will send a CAN Message from the Tx Buffer to the Rx Buffer
throuth internal loopback interconnect and print out the Message payload to terminal.

## Supported Boards
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/mscan/loopback/example_board_readme.md)
