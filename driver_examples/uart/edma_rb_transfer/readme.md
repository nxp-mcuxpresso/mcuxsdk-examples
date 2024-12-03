# uart_edma_rb_transfer

## Overview
The uart_edma ring buffer example shows how to use uart driver with EDMA:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board. And data from PC will be firstly 
stored to a ring buffer using EDMA methord, and than application will read the ring buffer 
in a certain time, such as 10ms. While data has already be stored in the ring buffer,
routine will send them back.

And the ring buffer is achieved based on the DMA's source address modulo feature, the base address
should be aligned to a 0-modulo-boundary.

## Supported Boards
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/uart/edma_rb_transfer/example_board_readme.md)
