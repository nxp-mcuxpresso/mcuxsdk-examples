# usart_transfer_sync_b2b_master

## Overview
The usart_interrupt_sync_transfer example shows how to use usart API in synchronous mode:

In this example, one usart instance will be selected as master ,and another as slave. The master 
will send data to slave in polling way, and slave will receive data in nonblocking way.
After all data has been received by slave, info will be printed by debug console.

## Supported Boards
- [LPCXpresso802](../../../../_boards/lpcxpresso802/driver_examples/usart/transfer_sync_b2b/master/example_board_readme.md)
- [LPCXpresso804](../../../../_boards/lpcxpresso804/driver_examples/usart/transfer_sync_b2b/master/example_board_readme.md)
