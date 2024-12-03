# flexcan_pretended_networking_wakeup

## Overview
The flexcan_pretended_networking_wakeup example shows how to wake up FLEXCAN module from Pretended Networking mode:

In this example, 2 boards are connected through CAN bus. Endpoint B need enter STOP mode first, then endpoint A(board A) send CAN Message to Endpoint B(board B)
when user press space key in terminal. Endpoint B will wake up from STOP mode after receive 4 specific wake up frame, and print
the message content to terminal.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
