# queued_spi_polling_b2b_transfer_slave

## Overview
The queued_spi_polling_b2b_transfer example shows how to use QUEUEDSPI driver in polling way:

In this example we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_polling_b2b_transfer_slave.c' includes the QUEUEDSPI slave code.

1. QUEUEDSPI master sends/receives data to/from QUEUEDSPI slave in polling. (QUEUEDSPI Slave uses interrupt to receive/send the data)

## Supported Boards
- [MC56F80000-EVK](../../../../_boards/mc56f80000evk/driver_examples/queued_spi/polling_b2b_transfer/slave/example_board_readme.md)
- [MC56F81000-EVK](../../../../_boards/mc56f81000evk/driver_examples/queued_spi/polling_b2b_transfer/slave/example_board_readme.md)
- [MC56F83000-EVK](../../../../_boards/mc56f83000evk/driver_examples/queued_spi/polling_b2b_transfer/slave/example_board_readme.md)
- [TWR-MC56F8200](../../../../_boards/twrmc56f8200/driver_examples/queued_spi/polling_b2b_transfer/slave/example_board_readme.md)
- [TWR-MC56F8400](../../../../_boards/twrmc56f8400/driver_examples/queued_spi/polling_b2b_transfer/slave/example_board_readme.md)
