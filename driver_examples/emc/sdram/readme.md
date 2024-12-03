# emc_sdram

## Overview

The emc_sdram example shows how to access the SDRAM.

In this example, user shall initialize the EMC (external memory controller), initialize the
EMC dynamic memory controller before access the external SDRAM.

If the sdram example fails, please make sure to check the following points:
1. Please take refer to the board.readme to check the jumper settings on your board.
2. Please take refer to the EMC chapter in API RM to do the delay calibration to found the best delay for your board, then update the delay to the EMC clock delay control registers in the system configure module registers.

## Supported Boards
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/emc/sdram/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/emc/sdram/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/emc/sdram/example_board_readme.md)
