# queued_spi_polling_b2b_transfer_master

## Overview
The queued_spi_polling_b2b_transfer examples show how to use QUEUEDSPI driver in polling way:

In this example we need two boards, one board is used as QUEUEDSPI master and another board is used as QUEUEDSPI slave.
The file 'queued_spi_polling_b2b_transfer_master.c' includes the QUEUEDSPI master code.

1. QUEUEDSPI master sends/receiveds data to/from QUEUEDSPI slave in polling. (QUEUEDSPI Slave uses interrupt to receive/send the data)

## Supported Boards
