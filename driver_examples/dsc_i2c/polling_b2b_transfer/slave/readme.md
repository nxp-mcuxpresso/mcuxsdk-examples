# dsc_i2c_polling_b2b_transfer_slave

## Overview
The i2c_polling_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a polling master:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [MC56F83000-EVK](../../../../_boards/mc56f83000evk/driver_examples/i2c/polling_b2b_transfer/slave/example_board_readme.md)
- [TWR-MC56F8200](../../../../_boards/twrmc56f8200/driver_examples/i2c/polling_b2b_transfer/slave/example_board_readme.md)
- [TWR-MC56F8400](../../../../_boards/twrmc56f8400/driver_examples/i2c/polling_b2b_transfer/slave/example_board_readme.md)
