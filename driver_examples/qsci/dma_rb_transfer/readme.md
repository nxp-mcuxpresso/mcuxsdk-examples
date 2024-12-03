# qsci_dma_rb_transfer

## Overview
The qsci_dma_rb_transfer example demonstrates the usage of the MSDK QSCI DMA driver. 
In this example, a ring buffer is used with DMA enabled, data received will be stored
in ring buffer first, while IDLE line is detected, the received characters will be read out,
and echo them back to the terminal using DMA method.

## Supported Boards
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/qsci/dma_rb_transfer/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/qsci/dma_rb_transfer/example_board_readme.md)
