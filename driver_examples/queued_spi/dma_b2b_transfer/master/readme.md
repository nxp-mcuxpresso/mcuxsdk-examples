# queued_spi_dma_b2b_transfer_master

## Overview
The queued_spi_dma_b2b_transfer example shows how to use QUEUEDSPI driver in dma way:

In this example, we need two boards, one board is used as QUEUEDSPI master and the other board is used as QUEUEDSPI slave.
The file 'queued_spi_dma_b2b_transfer_master.c' includes the QUEUEDSPI master code.

1. QUEUEDSPI master sends/receives data to/from QUEUEDSPI slave in dma way. (QUEUEDSPI Slave uses dma way to receive/send the data)

## Supported Boards
