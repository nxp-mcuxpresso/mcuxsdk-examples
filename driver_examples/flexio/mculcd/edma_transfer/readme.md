# flexio_mculcd_edma_transfer

## Overview
This example shows how to use flexio mculcd edma driver to drive MCU LCD panel.
Please focus on functions DEMO_LcdWriteCommand, DEMO_LcdWriteData, and DEMO_LcdWriteMemory
to learn how to use the APIs. Note that the DEMO_LcdWriteCommand and DEMO_LcdWriteData
still use blocking APIs, because the send data is very small, blocking API is faster
in this case. It is still OK to use FLEXIO_MCULCD_TransferEDMA in this case.

In this project, the panel is devided into 4 windows, the project changes the color
of each window one by one. The behavior is:
1. Project starts with black screen.
2. Fill window one by one as red, and delay 0.5s after fill each window.
3. Fill window one by one as green, and delay 0.5s after fill each window.
4. Fill window one by one as blue, and delay 0.5s after fill each window.
5. Fill window one by one as white, and delay 0.5s after fill each window.
6. Go to step 2.

+---------------------------+---------------------------+
|                           |                           |
|                           |                           |
|                           |                           |
|      window 0             |        window 1           |
|                           |                           |
|                           |                           |
+---------------------------+---------------------------+
|                           |                           |
|                           |                           |
|                           |                           |
|      window 2             |        window 3           |
|                           |                           |
|                           |                           |
+---------------------------+---------------------------+

## Supported Boards
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
- [FRDM-MCXA276](../../../../_boards/frdmmcxa276/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/flexio/mculcd/edma_transfer/example_board_readme.md)
