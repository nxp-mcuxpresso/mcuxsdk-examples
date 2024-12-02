# gdma_transfer_lli

## Overview
This project shows how to use GDMA transactional APIs to do the memory to memory
data transfer with link list feature.

In this example, there are 4 buffers, s_gdmaData0 to s_gdmaData3. At the beginning,
only s_gdmaData0 is filled with data. GDMA copies s_gdmaData0 to s_gdmaData1,
then copies s_gdmaData1 to s_gdmaData2, and copies s_gdmaData2 to s_gdmaData3.
So at last the data in 4 buffers should be the same.

The project checks the data in each buffer and show the result terminal.

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/gdma/transfer_lli/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/gdma/transfer_lli/example_board_readme.md)
