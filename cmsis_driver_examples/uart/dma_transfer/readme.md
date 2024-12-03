# cmsis_uart_dma_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware
that implements for example communication stacks, file systems, or graphic user interfaces.
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_uart_edma_transfer example shows how to use uart cmsis  driver with EDMA:

In this example, one uart instance connect to PC through uart, the board will
send back all characters that PC send to the board.

Note: The example echo every 8 characters, so input 8 characters every time.

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/cmsis_driver_examples/uart/dma_transfer/example_board_readme.md)
