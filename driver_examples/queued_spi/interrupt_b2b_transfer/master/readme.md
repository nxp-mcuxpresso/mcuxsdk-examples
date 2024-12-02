# queued_spi_interrupt_b2b_transfer_master

## Overview
The queued_spi_interrupt_b2b_transfer examples show how to use QUEUEDSPI driver in interrupt way:

In this example we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_interrupt_b2b_transfer_master.c' includes the QUEUEDSPI master code.
This example uses the transactional API in QUEUEDSPI driver.

1. QUEUEDSPI master sends/receives data to/from DSPI slave in interrupt. (QUEUEDSPI Slave uses interrupt to receive/send the data)

## Supported Boards
