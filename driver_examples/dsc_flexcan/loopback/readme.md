# dsc_flexcan_loopback

## Overview
The flexcan_loopback_functional example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Rx Message Buffer and the other FlexCAN Message Buffer to Tx Message Buffer with same ID.
After that, the example will send a CAN Message from the Tx Message Buffer to the Rx Message Buffer
throuth internal loopback interconnect and print out the Message payload to terminal.

## Supported Boards
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/flexcan/loopback/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/flexcan/loopback/example_board_readme.md)
