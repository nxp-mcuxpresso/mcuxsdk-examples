# flexcan_efifo_interrupt_transfer_low_level

## Overview
The flexcan_efifo_interrupt_transfer_low_level example shows how to use FlexCAN Enhanced Rx FIFO with low-level hardware APIs in none-blocking interrupt way:

This example demonstrates direct hardware access without using the FlexCAN transfer layer. It implements a custom ISR that calls hardware APIs directly to handle Enhanced Rx FIFO interrupts efficiently. The ISR uses a ring buffer design to manage received frames between interrupt context and main loop.

The example will config one FlexCAN Message Buffer to Tx Message Buffer and setup Enhanced Rx FIFO. After that, the example will send some CAN FD Messages(default 4 frames) from Tx Message Buffer to Enhanced Rx FIFO through internal loopback interconnect. The sent and received message will be print out to terminal

2 boards are required to be connected through CAN bus. Endpoint A(board A) send a CAN Message to Endpoint B(board B) when user press 
space key in terminal. Endpoint B receives the message by Enhanced Rx FIFO, and print the message content to terminal after receive CAN FD messages.

## Supported Boards
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/efifo_interrupt_transfer_low_level/example_board_readme.md)
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/flexcan/efifo_interrupt_transfer_low_level/example_board_readme.md)
