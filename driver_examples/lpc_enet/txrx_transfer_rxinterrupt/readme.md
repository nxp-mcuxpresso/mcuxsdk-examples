# enet_txrx_transfer_rxinterrupt

## Overview

The enet_rxtx_rxinterrupt example shows the simplest way to use ENET transactional tx/rx API for simple frame receive and transmit.

1. This example shows how to initialize the ENET.
2. How to use ENET to receive frame in interrupt irq handler and to transmit frame.

The example transmits 20 number broadcast frame, print the number of recieved frames. To avoid
the receive number overflow, the transmit/receive loop with automatically break when 20 number
are received.

## Supported Boards
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/enet/txrx_transfer_rxinterrupt/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/enet/txrx_transfer_rxinterrupt/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/enet/txrx_transfer_rxinterrupt/example_board_readme.md)
