# edma3_channel_link

## Overview
The EDMA channel link example is a simple demonstration program that uses the SDK software.
It excuates channel link transfer using the SDK EDMA drivers.
The purpose of this example is to show how to use the EDMA and to provide a simple example for
debugging and further development, it demostrates how to use the minor loop link and major loop link:
Since each transfer request can be divided into MAJOR_LOOPS_COUNTS * MINOR_LOOPS_BYTES,
such as you want to request EDMA transfer 8 bytes total, 4 bytes each request, then MAJOR_LOOPS_COUNTS = 2, MINOR_LOOPS_BYTES = 4.
The minor loop channel linking occurs at the completion of the minor loop 4 byte transferred.
The major loop channel linking is occurs at the major loop exhausted.
The example demostrate the channel link transfer by the feature of edma4:

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/edma3/channel_link/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/edma3/channel_link/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/edma3/channel_link/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/edma3/channel_link/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/edma3/channel_link/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/edma3/channel_link/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/edma3/channel_link/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/edma3/channel_link/example_board_readme.md)
