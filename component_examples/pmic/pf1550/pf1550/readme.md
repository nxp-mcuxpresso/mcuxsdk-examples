# pf1550

## Overview
The pf1550 driver example demonstrates the usage of pf1550 SDK component driver.
The example shows the usage of PF1550 API to:
1. Set regulator output voltages;
2. Query regulator output voltages;
3. Dump PF1550 register content;
4. Charge a Li-on Battery Cell.

To use this example, user need to pay attention the output voltage when update the regulator
output and make sure the output voltage to set will not cause hardware damage;
You need also to pay attention to the charging voltage setting when charging Li-on battery cell,
and make sure that the battery is not over-charged.

## Supported Boards
- [EVK-MCIMX7ULP](../../../../_boards/evkmcimx7ulp/component_examples/pf1550/example_board_readme.md)
