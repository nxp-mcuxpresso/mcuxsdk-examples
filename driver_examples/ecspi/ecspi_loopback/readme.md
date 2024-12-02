# ecspi_loopback

## Overview
The ecspi_loopback demo shows how the ecspi do a loopback transfer internally.
The ECSPI connects the transmitter and receiver sections internally, and the data shifted out from the 
most-significant bit of the shift register is looped back into the least-significant bit of the Shift register. 
In this way, a self-test of the complete transmit/receive path can be made. The output pins are not affected, 
and the input pins are ignored.

## Supported Boards
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/driver_examples/ecspi/ecspi_loopback/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/driver_examples/ecspi/ecspi_loopback/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/ecspi/ecspi_loopback/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/driver_examples/ecspi/ecspi_loopback/example_board_readme.md)
