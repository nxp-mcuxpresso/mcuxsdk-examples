# dspi_dma_b2b_transfer_master

## Overview
The dspi_dma_b2b_transfer example shows how to use DSPI driver in dma way:

In this example , we need two boards, one board used as DSPI master and another board used as DSPI slave.
The file 'dspi_dma_b2b_transfer_master.c' includes the DSPI master code.

1. DSPI on master board send/received data to/from DSPI on slave board in dma.
2. DSPI on slave board using dma to receive/send the data
