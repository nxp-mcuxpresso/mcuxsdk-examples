# ecspi_interrupt_b2b_transfer_slave

## Overview
The ecspi_interrupt_b2b_transfer example shows how to use ECSPI driver in interrupt way:

In this example , we need two boards, one board used as ECSPI master and another board used as ECSPI slave.
The file 'ecspi_interrupt_b2b_transfer_slave.c' includes the ECSPI slave code.
This example uses the transactional API in ECSPI driver.

1. ECSPI master send/received data to/from ECSPI slave in interrupt . (ECSPI Slave using interrupt to receive/send the data)

## Supported Boards
- [EVK-MIMX8MM](../../../../_boards/evkmimx8mm/driver_examples/ecspi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MN](../../../../_boards/evkmimx8mn/driver_examples/ecspi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../../_boards/evkmimx8mnddr3l/driver_examples/ecspi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [EVK-MIMX8MP](../../../../_boards/evkmimx8mp/driver_examples/ecspi/interrupt_b2b_transfer/slave/example_board_readme.md)
