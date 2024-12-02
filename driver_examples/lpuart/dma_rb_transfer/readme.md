# lpuart_dma_rb_transfer

## Overview
The lpuart_dma Ring Buffer Example project is to demonstrate usage of the KSDK lpuart driver.
In the example, a ring buffer will be used with DMA enabled, data received will be stored in ring 
buffer first, while IDLE line was detected, routine will read out the received characters, and echo back
them to the terminal by using DMA mode. so, you can see the your pressed characters on the terminal.

And the ring buffer is achieved based on the DMA's source address modulo feature, the base address
should be aligned to a 0-modulo-boundary.

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/lpuart/dma_rb_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/lpuart/dma_rb_transfer/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/driver_examples/lpuart/dma_rb_transfer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/lpuart/dma_rb_transfer/example_board_readme.md)
