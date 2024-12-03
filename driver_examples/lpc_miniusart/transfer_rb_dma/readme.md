# usart_transfer_rb_dma

## Overview
The usart dma ring buffer example shows how to use usart driver with DMA driver used:

In this example, a ring buffer was implemented to receive the characters from PC side, 
to implemente the ring buffer, a descriptor was created and chain itself as the next descriptor,
then the DMA transfer will start a continuous transfer.

While data in the ring buffer reach 8 characters, routine will send them out using DMA mode.

## Supported Boards
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/usart/transfer_rb_dma/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/usart/transfer_rb_dma/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/usart/transfer_rb_dma/example_board_readme.md)
