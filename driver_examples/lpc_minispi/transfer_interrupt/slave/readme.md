# spi_transfer_interrupt_slave

## Overview
The spi_interrupt_transfer_slave example shows how to use spi driver as slave to receive data from master.

In this example, one spi instance as slave and another spi instance on other board as master. 
Master sends a piece of data to slave, and receive a piece of data from slave. This example 
checks if the data received from master is correct. This example should work with 
spi_interrupt_transfer_master example. And this example should start first.

## Supported Boards
- [LPC845BREAKOUT](../../../../_boards/lpc845breakout/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
- [LPCXpresso802](../../../../_boards/lpcxpresso802/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
- [LPCXpresso804](../../../../_boards/lpcxpresso804/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
- [LPCXpresso824MAX](../../../../_boards/lpcxpresso824max/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
- [LPCXpresso845MAX](../../../../_boards/lpcxpresso845max/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/spi/transfer_interrupt/slave/example_board_readme.md)
