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
- [FRDM-MCXA346](../../../_boards/frdmmcxa346/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXE247](../../../_boards/frdmmcxe247/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCX-W71-EVK](../../../_boards/mcxw71evk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA174](../../../_boards/frdmmcxa174/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA344](../../../_boards/frdmmcxa344/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [KW47-LOC](../../../_boards/kw47loc/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [MCXW72-LOC](../../../_boards/mcxw72loc/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
- [FRDM-MCXW72](../../../_boards/frdmmcxw72/driver_examples/flexcan/pretended_networking_wakeup/example_board_readme.md)
