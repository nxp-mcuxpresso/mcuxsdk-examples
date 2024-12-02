# mcan_loopback

## Overview
The mcan_loopback example shows how to use the loopback test mode to debug your CAN Bus design:

To demonstrate this example, only one board is needed. The example will config Tx Buffer to send
and Rx Fifo to receive. After that, the example will send a CAN Message throuth internal loopback
interconnect and print out the Message payload to the terminal.

## Supported Boards
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/mcan/loopback/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/mcan/loopback/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/mcan/loopback/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/mcan/loopback/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/mcan/loopback/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/mcan/loopback/example_board_readme.md)
