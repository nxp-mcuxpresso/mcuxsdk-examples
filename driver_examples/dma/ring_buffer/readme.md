# dma_ring_buffer

## Overview
The DMA ring buffer example is a simple demonstration program that uses the SDK software.It demostrates how to implement ring buffer using the SDK DMA drivers.The purpose of this example is to show how to use the DMA and to provide a simple example for debugging and further development.
There are two channels involved in this demo:
1. channel 0 is used for handling the ring buffer transfer
2. channel 1 is used for restart channel 0.

+-------+     +-----------------------------+
| start | --> |     channel 0 transfer      | <+
+-------+     +-----------------------------+  |
                |                              |
                | finished                     |
                v                              |
              +-----------------------------+  |
              | channel 1 restart channel 0 | -+
              +-----------------------------+

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/dma/ring_buffer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/dma/ring_buffer/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/driver_examples/dma/ring_buffer/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/driver_examples/dma/ring_buffer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/dma/ring_buffer/example_board_readme.md)
