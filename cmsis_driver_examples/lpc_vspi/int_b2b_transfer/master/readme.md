# cmsis_spi_int_b2b_transfer_master

## Overview
The cmsis_spi_int_b2b_transfer example shows how to use SPI CMSIS driver in interrupt way:

In this example , we need two boards, one board used as SPI master and another board used as SPI slave.
The file 'cmsis_spi_int_b2b_transfer_master.c' includes the SPI master code.

1. SPI master send/received data to/from SPI slave in interrupt way. 