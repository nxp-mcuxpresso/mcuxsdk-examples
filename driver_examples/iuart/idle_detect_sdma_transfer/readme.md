# iuart_idle_detect_sdma_transfer

## Overview
The uart_idle_detect_sdma example shows how to use uart driver in sdma way:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Uart will receive 8 characters every time, but if the character is less then 8, the idle
line interrupt will generate, and abort the SDMA receive operation, and send out the received
characters.

## Supported Boards
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/driver_examples/uart/idle_detect_sdma_transfer/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/driver_examples/uart/idle_detect_sdma_transfer/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/driver_examples/uart/idle_detect_sdma_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/uart/idle_detect_sdma_transfer/example_board_readme.md)
