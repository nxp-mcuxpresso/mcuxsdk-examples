# mcan_interrupt_transfer

## Overview
The mcan_interrupt example shows how to use MCAN driver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when users press space key in terminal. Endpoint B receives the message, prints
the message content to terminal and echoes back the message. Endpoint A will increase the received
message and wait for the next transmission the users initiate.

## Supported Boards
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/mcan/interrupt_transfer/example_board_readme.md)
