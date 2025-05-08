# cmu_fc

## Overview
The cmu_fc example is to demonstrate usage of the SDK cmu_fc driver to check if the target clock frequency is within the threshold range. In this example, the low threshold is specified as the same as the high threshold to trigger a frequency lower than low frequency reference threshold event, which leads to a destructive reset. Then use the optimum high and low threshold to validate the target clock is operating at the correct frequency.

## Supported Boards
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/driver_examples/cmu_fc/example_board_readme.md)