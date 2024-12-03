# cmsis_ii2c_int_b2b_transfer_slave

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware
that implements for example communication stacks, file systems, or graphic user interfaces.
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The i2c_interrupt_b2b_transfer_master example shows how to use CMSIS i2c driver as master to do board to board transfer
with interrupt:

In this example, one i2c instance as master and another i2c instance on the other board as slave. Master sends a
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from
slave is correct.

## Supported Boards
- [EVK-MIMX8MM](../../../../_boards/evkmimx8mm/cmsis_driver_examples/i2c/int_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MN](../../../../_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../../_boards/evkmimx8mnddr3l/cmsis_driver_examples/i2c/int_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MP](../../../../_boards/evkmimx8mp/cmsis_driver_examples/i2c/int_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MQ](../../../../_boards/evkmimx8mq/cmsis_driver_examples/i2c/int_b2b_transfer/slave/example_board_readme.md)
