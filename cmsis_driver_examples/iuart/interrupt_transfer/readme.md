# cmsis_iuart_interrupt_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware
that implements for example communication stacks, file systems, or graphic user interfaces.
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_uart_interrupt_transfer example shows how to use uart cmsis driver in interrupt way:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

## Supported Boards
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/cmsis_driver_examples/uart/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/cmsis_driver_examples/uart/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/cmsis_driver_examples/uart/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/cmsis_driver_examples/uart/interrupt_transfer/example_board_readme.md)
