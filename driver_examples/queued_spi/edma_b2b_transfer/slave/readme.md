# queued_spi_edma_b2b_transfer_slave

## Overview
The queued_spi_edma_b2b_transfer example shows how to use QUEUEDSPI driver in edma way:

In this example, we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_edma_b2b_transfer_slave.c' includes the DSPI slave code.

1. QUEUEDSPI master sends/receives data to/from QUEUEDSPI slave in edma way. (QUEUEDSPI Slave uses edma way to receive/send the data)

## Supported Boards
