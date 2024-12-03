# dma_channel_link

## Overview
The DMA channel link example is a simple demonstration program that uses the SDK software.
It executes channel link transfer using the SDK DMA drivers.
The purpose of this example is to show how to use the DMA and to provide a simple example for
debugging and further development.
                  not finished                                   not finished
                +----------------+                             +----------------+
                v                |                             v                |
+-------+     +--------------------+  each request trigger   +--------------------+  finished   +------------------+
| start | --> | channel 0 transfer | ----------------------> | channel 1 transfer | ----------> | example complete |
+-------+     +--------------------+                         +--------------------+             +------------------+

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/dma/channel_link/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/dma/channel_link/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/driver_examples/dma/channel_link/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/driver_examples/dma/channel_link/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/dma/channel_link/example_board_readme.md)
