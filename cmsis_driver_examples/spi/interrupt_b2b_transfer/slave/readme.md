# cmsis_spi_interrupt_b2b_transfer_slave

## Overview
The spi_interrupt_b2b_transfer example shows how to use SPI CMSIS driver in interrupt way:

In this example , we need two boards, one board used as SPI master and another board used as SPI slave.
The file 'spi_interrupt_b2b_transfer_slave.c' includes the SPI slave code.
This example uses the transactional API in SPI driver.

1. SPI master send/received data to/from SPI slave in interrupt .

## Supported Boards
- [FRDM-K32L2B](../../../../_boards/frdmk32l2b/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [FRDM-KE02Z40M](../../../../_boards/frdmke02z40m/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC041](../../../../_boards/frdmmcxc041/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC242](../../../../_boards/frdmmcxc242/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXC444](../../../../_boards/frdmmcxc444/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../../_boards/twrkm34z50mv3/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM34Z75M](../../../../_boards/twrkm34z75m/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
- [TWR-KM35Z75M](../../../../_boards/twrkm35z75m/cmsis_driver_examples/spi/interrupt_b2b_transfer/slave/example_board_readme.md)
