# mcx_enet_txrx_rxinterrupt

## Overview

The enet_rxtx_rxinterrupt example shows the simplest way to use ENET functional tx/rx API for simple frame receive and transmit.

1. This example shows how to initialize the ENET.
2. How to use ENET to receive frame in interrupt irq handler and to transmit frame.

The example transmits 20 number broadcast frame, print the number of recieved frames. To avoid
the receive number overflow, the transmit/receive loop with automatically break when 20 number
are received.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/enet/txrx_rxinterrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/enet/txrx_rxinterrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/enet/txrx_rxinterrupt/example_board_readme.md)
