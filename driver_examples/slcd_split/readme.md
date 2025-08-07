# slcd

## Overview

The SLCD example shows how to use SLCD driver on a multocore device.
## Running the demo

In this example, the SLCD displays as below first on primary core then on secondary core.
1. Show digital numbers.
2. Show letters. Since not all letters can be shown by 7-segment panel, only the possible ones are shown.
3. Show the icons one by one.
4. All segments on, blink, then turn off.

## Note

User can attach to the running application and start debugging only after the secondary
core application has been started.

## Supported Boards
- [FRDM-MCXL255](../../_boards/frdmmcxl255/driver_examples/slcd_split/example_board_readme.md)
