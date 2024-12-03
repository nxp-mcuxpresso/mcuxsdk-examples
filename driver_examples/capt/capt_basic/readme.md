# capt_basic

## Overview
This example shows how to capture touch data using CAPT poll-now mode.
In this mode, application trigger the polling round, after polling round, the
CAPT stops and notifies application by interrupt or register status.
In this example, application poll the enabled pins one by one, after all pins
are sampled, the result are shown in the debug terminal. Then application
starts to sample all the enabled pins again.

When the example runs, the sampled data is shown in the terminal. When electrode
plate is touched, the sampled data changes too.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/capt/capt_basic/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/capt/capt_basic/example_board_readme.md)
