# i3c_interrupt_b2b_slave

## Overview
The example shows how to use i3c driver as slave to do board to board transfer with a interrupt master, slave application is
implemented with functional API instead of transactional API to reduce code size and improve performance on low frequency MCU.

In this example, one i3c instance as slave and another i3c instance on the other board as master. Master will enter dynamic
address cycle to assign address to the connected slave, after success, will use I3C SDR mode to transfer data, then receive
data from the connected I3C slave and check the data consistency.

## Supported Boards
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i3c/interrupt_b2b/slave/example_board_readme.md)
