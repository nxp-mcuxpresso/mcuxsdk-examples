# lin_master

## Overview
The lin_master demo application demonstrates how to use LIN bus signal transfer from master node.

This demo application is expected to work with another LIN instance implemented as slave node on the other board. Master node repeatedly initializes transfer by generating frame header, master and slave then exchange response as signal represented by the character array. This demo uses auto-baudrate feature.

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lin/master/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lin/master/example_board_readme.md)
