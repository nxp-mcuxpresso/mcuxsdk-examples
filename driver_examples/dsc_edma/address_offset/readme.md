# edma_address_offset

## Overview
The EDMA address offset example is a simple demonstration program that uses the SDK software.
It excuates a simple transfer from source buffer to destination buffer using the SDK EDMA drivers.
The purpose of this example is to show how to use the EDMA minor loop/major loop offset feature and to provide a simple example for
debugging and further development.
Note: The examples is also trying to show when minor loop offset and major loop offset are both enabled for a channel, the last minor loop offset will not applied to address
and the major loop offset will applied to the address.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/edma/address_offset/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/edma/address_offset/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/edma/address_offset/example_board_readme.md)
