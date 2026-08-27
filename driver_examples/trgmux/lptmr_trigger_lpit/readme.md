# trgmux_lptmr_trigger_lpit

## Overview

The lptmr_trigger_lpit project is a simple example of the SDK LPIT, LPTMR and TRGMUX driver which
shows how to use the LPTMR and TRGMUX to generate a LPIT trigger. It sets up the LPTMR hardware
block to generate the trigger every 0.5 second, and the LPIT counter will load on the first trigger
rising edge and then decrement down to zero on each trigger rising edge. A message would be printed
on the serial terminal.

## Supported Boards
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/trgmux/lptmr_trigger_lpit/example_board_readme.md)
- [FRDM-MCXE247](../../../_boards/frdmmcxe247/driver_examples/trgmux/lptmr_trigger_lpit/example_board_readme.md)
