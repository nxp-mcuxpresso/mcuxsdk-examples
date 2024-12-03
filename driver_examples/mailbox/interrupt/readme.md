# mailbox_interrupt

## Overview
The mailbox_interrupt example shows how to use mailbox to exchange message.

In this example:
The primary core writes a value to the secondary core mailbox, it causes mailbox interrupt
on the secondary core side. The secondary core reads the value from mailbox, it increments and writes it to mailbox register
for the primary core, which causes mailbox interrupt on the primary core side. The primary core reads value from mailbox, 
it increments and writes it to the mailbox register for the secondary core again.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/mailbox/interrupt/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/mailbox/interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/mailbox/interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/mailbox/interrupt/example_board_readme.md)
