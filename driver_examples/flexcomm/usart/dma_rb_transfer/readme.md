# usart_dma_rb_transfer

## Overview
The usart dma ring buffer example shows how to use usart driver with DMA driver used:

In this example, a ring buffer was implemented to receive the characters from PC side, 
to implemente the ring buffer, a descriptor was created and chain itself as the next descriptor,
then the DMA transfer will start a continuous transfer.

While data in the ring buffer reach 8 characters, routine will send them out using DMA mode.

## Supported Boards
- [LPCXpresso51U68](../../../../_boards/lpcxpresso51u68/driver_examples/usart/dma_rb_transfer/example_board_readme.md)
- [LPCXpresso54628](../../../../_boards/lpcxpresso54628/driver_examples/usart/dma_rb_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../../_boards/lpcxpresso54s018/driver_examples/usart/dma_rb_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../../_boards/lpcxpresso54s018m/driver_examples/usart/dma_rb_transfer/example_board_readme.md)
