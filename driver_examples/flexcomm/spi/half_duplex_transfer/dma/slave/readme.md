# spi_half_duplex_dma_slave

## Overview
The spi_half_duplex_dma transfer_slave example shows how to receive and transmit data to master board.

Notice: The SPI slave of this example uses dma mode. The data transfer size(not buffer size) is twice of
the master's transfer size. The first half of the transmission will receive data, and the second half will send
data back to master, so the address of the receive buffer is &rxDataPolling[64].

In this example, one spi instance as slave and another spi instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
master is correct. And slave will print what it received.

## Supported Boards
- [LPCXpresso54628](../../../../../../_boards/lpcxpresso54628/driver_examples/spi/half_duplex_transfer/dma/slave/example_board_readme.md)
- [LPCXpresso54S018](../../../../../../_boards/lpcxpresso54s018/driver_examples/spi/half_duplex_transfer/dma/slave/example_board_readme.md)
- [LPCXpresso54S018M](../../../../../../_boards/lpcxpresso54s018m/driver_examples/spi/half_duplex_transfer/dma/slave/example_board_readme.md)
