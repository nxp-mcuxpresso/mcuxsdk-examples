# ii2c_interrupt_b2b_transfer_slave

## Overview
The i2c_interrupt_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with interrupt:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [EVK-MIMX8MM](../../../../_boards/evkmimx8mm/driver_examples/i2c/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MN](../../../../_boards/evkmimx8mn/driver_examples/i2c/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../../_boards/evkmimx8mnddr3l/driver_examples/i2c/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MP](../../../../_boards/evkmimx8mp/driver_examples/i2c/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MQ](../../../../_boards/evkmimx8mq/driver_examples/i2c/interrupt_b2b_transfer/slave/example_board_readme.md)
