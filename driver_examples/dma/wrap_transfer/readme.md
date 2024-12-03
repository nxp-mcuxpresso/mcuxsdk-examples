# dma_wrap_transfer

## Overview
The DMA wrap transfer example is a simple demonstration program that uses the SDK software.
It excuates wrap transfer from source buffer to destination buffer using the SDK EDMA drivers.
The purpose of this example is to show how to use the wrap feature of EDMA and to provide a simple example for
debugging and further development.
                +--------------------------------------------------------------+
                v                                                              |
+-------+     +--------------------+  source address reach modulo boundary   +---------------------+
| start | --> | channel 0 transfer | --------------------------------------> | source address wrap |
+-------+     +--------------------+                                         +---------------------+
                |
                | finsihed
                v
              +--------------------+
              |  example complete  |
              +--------------------+

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/dma/wrap_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/dma/wrap_transfer/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/driver_examples/dma/wrap_transfer/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/driver_examples/dma/wrap_transfer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/dma/wrap_transfer/example_board_readme.md)
