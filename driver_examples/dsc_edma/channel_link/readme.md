# dsc_edma_channel_link

## Overview
The EDMA channel link example is a simple demonstration program that uses the SDK software.
It excuates mempry to memory transfer from source buffer to destination buffer using the SDK EDMA drivers.
The purpose of this example is to show how to use the EDMA major loop/minor loop channel link feature and to provide a simple example for
debugging and further development.
Note: The examples is also trying to show when minor loop link and major loop link are both enabled for a channel, the last minor loop link will not take effect
and the major loop link will be triggered.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/edma/channel_link/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/edma/channel_link/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/edma/channel_link/example_board_readme.md)
