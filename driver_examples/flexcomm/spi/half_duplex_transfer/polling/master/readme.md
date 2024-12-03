# spi_half_duplex_polling_master

## Overview
The spi_half_duplex_polling_transfer_master example shows how to use driver API to transfer in half-duplex way.  

In this example, one spi instance as master and another spi instance on the othere board as slave. Master sends 
a piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.
Besides, master will transfer data in polling way.

## Supported Boards
- [LPCXpresso51U68](../../../../../../_boards/lpcxpresso51u68/driver_examples/spi/half_duplex_transfer/polling/master/example_board_readme.md)
- [LPCXpresso54628](../../../../../../_boards/lpcxpresso54628/driver_examples/spi/half_duplex_transfer/polling/master/example_board_readme.md)
- [LPCXpresso54S018](../../../../../../_boards/lpcxpresso54s018/driver_examples/spi/half_duplex_transfer/polling/master/example_board_readme.md)
- [LPCXpresso54S018M](../../../../../../_boards/lpcxpresso54s018m/driver_examples/spi/half_duplex_transfer/polling/master/example_board_readme.md)
