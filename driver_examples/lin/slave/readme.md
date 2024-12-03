# lin_slave

## Overview
The lin_master demo application demonstrates how to use LIN bus signal transfer from slave node.

This demo application is expected to work with another LIN instance implemented as master node on the other board. Slave is waiting for master to initialize the communication then exchange a response as signal represented by the character array. This demo uses auto-baudrate feature.

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lin/slave/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lin/slave/example_board_readme.md)
