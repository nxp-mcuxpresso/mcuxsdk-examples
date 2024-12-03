# mailbox_mutex

## Overview
The mailbox_mutex example shows how to use mailbox mutex.

In this example:
The core 0 sends address of shared variable to core 1 by mailbox.
Both cores trying to get mutex in while loop, after that updates shared variable
and sets mutex, to allow access other core to shared variable.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/mailbox/mutex/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/mailbox/mutex/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/mailbox/mutex/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/mailbox/mutex/example_board_readme.md)
