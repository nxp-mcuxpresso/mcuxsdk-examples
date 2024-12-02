# usart_transfer_sync_mode

## Overview
The usart_interrupt_sync_transfer example shows how to use usart API in synchronous mode:

In this example, one usart instance will be selected as master ,and another as slave. The master 
will send data to slave in polling way, and slave will receive data in nonblocking way.
After all data has been received by slave, info will be printed by debug console.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/usart/transfer_sync_mode/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/usart/transfer_sync_mode/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/usart/transfer_sync_mode/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/usart/transfer_sync_mode/example_board_readme.md)
