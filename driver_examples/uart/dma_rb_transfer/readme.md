# uart_dma_rb_transfer

## Overview
The uart_dma ring buffer example shows how to use uart driver with DMA:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board. And data from PC will be firstly 
stored to a ring buffer using DMA methord, and than application will read the ring buffer 
in a certain time, such as 10ms. While data has already be stored in the ring buffer,
routine will send them back.

And the ring buffer is achieved based on the DMA's source address modulo feature, the base address
should be aligned to a 0-modulo-boundary.

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/uart/dma_rb_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/uart/dma_rb_transfer/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/driver_examples/uart/dma_rb_transfer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/uart/dma_rb_transfer/example_board_readme.md)
