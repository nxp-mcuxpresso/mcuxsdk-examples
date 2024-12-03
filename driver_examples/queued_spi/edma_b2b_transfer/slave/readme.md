# queued_spi_edma_b2b_transfer_slave

## Overview
The queued_spi_edma_b2b_transfer example shows how to use QUEUEDSPI driver in edma way:

In this example, we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_edma_b2b_transfer_slave.c' includes the DSPI slave code.

1. QUEUEDSPI master sends/receives data to/from QUEUEDSPI slave in edma way. (QUEUEDSPI Slave uses edma way to receive/send the data)

## Supported Boards
- [MC56F80000-EVK](../../../../_boards/mc56f80000evk/driver_examples/queued_spi/edma_b2b_transfer/slave/example_board_readme.md)
- [MC56F81000-EVK](../../../../_boards/mc56f81000evk/driver_examples/queued_spi/edma_b2b_transfer/slave/example_board_readme.md)
- [MC56F83000-EVK](../../../../_boards/mc56f83000evk/driver_examples/queued_spi/edma_b2b_transfer/slave/example_board_readme.md)
