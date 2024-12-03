# i3c_interrupt_b2b_master

## Overview
The example shows how to use i3c driver as master to do board to board data communication. The master will call
functional APIs to construct the data transfer flow instead of using transactional API, this aims to reduce code
size and acheive higher performance on low frequency MCUs.

In this example, one i3c instance as master and another i3c instance on the other board as slave. The master will
firstly enter dynamic address cycle to assign address to the connected slave, after success, will use I3C SDR mode
to transfer data, then receive data from the connected I3C slave and check the data consistency.

## Supported Boards
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i3c/interrupt_b2b/master/example_board_readme.md)
