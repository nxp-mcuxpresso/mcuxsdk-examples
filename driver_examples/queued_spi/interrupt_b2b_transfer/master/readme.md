# queued_spi_interrupt_b2b_transfer_master

## Overview
The queued_spi_interrupt_b2b_transfer examples show how to use QUEUEDSPI driver in interrupt way:

In this example we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_interrupt_b2b_transfer_master.c' includes the QUEUEDSPI master code.
This example uses the transactional API in QUEUEDSPI driver.

1. QUEUEDSPI master sends/receives data to/from DSPI slave in interrupt. (QUEUEDSPI Slave uses interrupt to receive/send the data)

## Supported Boards
- [MC56F80000-EVK](../../../../_boards/mc56f80000evk/driver_examples/queued_spi/interrupt_b2b_transfer/master/example_board_readme.md)
- [MC56F81000-EVK](../../../../_boards/mc56f81000evk/driver_examples/queued_spi/interrupt_b2b_transfer/master/example_board_readme.md)
- [MC56F83000-EVK](../../../../_boards/mc56f83000evk/driver_examples/queued_spi/interrupt_b2b_transfer/master/example_board_readme.md)
- [TWR-MC56F8200](../../../../_boards/twrmc56f8200/driver_examples/queued_spi/interrupt_b2b_transfer/master/example_board_readme.md)
- [TWR-MC56F8400](../../../../_boards/twrmc56f8400/driver_examples/queued_spi/interrupt_b2b_transfer/master/example_board_readme.md)
