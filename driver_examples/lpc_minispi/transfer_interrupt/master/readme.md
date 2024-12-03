# spi_transfer_interrupt_master

## Overview
The spi_interrupt_transfer_master example shows how to use spi driver as master to do board to board
transfer in interrupt way:

In this example, one spi instance as master and another spi instance on othere board as slave. Master
sends a piece of data to slave, and receive a piece of data from slave. This example checks if the 
data received from slave is correct. This example need to work with spi_interrupt_transfer_slave example.

## Supported Boards
- [LPC845BREAKOUT](../../../../_boards/lpc845breakout/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
- [LPCXpresso802](../../../../_boards/lpcxpresso802/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
- [LPCXpresso804](../../../../_boards/lpcxpresso804/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
- [LPCXpresso824MAX](../../../../_boards/lpcxpresso824max/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
- [LPCXpresso845MAX](../../../../_boards/lpcxpresso845max/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/spi/transfer_interrupt/master/example_board_readme.md)
