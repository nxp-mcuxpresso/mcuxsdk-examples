# gdma_transfer_wrap

## Overview
This project shows how to use GDMA transactional APIs to do the wrap transfer.

Both the source address and destination address can be wrapped.

In this example, there are 2 buffer, s_wrappedData is 16-byte buffer,
s_unwrappedData is 64-byte buffer.

In case APP_GdmaSrcWrap, s_wrappedData is filled with 0, 1, 2, ..., 15,
s_unwrappedData is empty. GDMA copies s_wrappedData to s_unwrappedData, and the
source address wrap is set to 16. When transfer finished, the s_unwrappedData is:
0, 1, 2, ..., 15, 0, 1, 2, ..., 15, 0, 1, 2, ..., 15, 0, 1, 2, ..., 15.

In case APP_GdmaDestWrap, s_unwrappedData is filled with 0, 1, 2, ..., 63,
s_wrappedData is empty. GDMA copies s_unwrappedData to s_wrappedData, and the
destination address wrap is set to 16. When transfer finished, the s_wrappedData is:
48, 49, 50, ..., 63.

The project checks the data and shows the result in terminal.

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/gdma/transfer_wrap/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/gdma/transfer_wrap/example_board_readme.md)
