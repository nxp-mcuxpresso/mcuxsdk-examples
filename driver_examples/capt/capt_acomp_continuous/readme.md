# capt_acomp_continuous

## Overview
This example shows how to capture touch data using CAPT continuous mode.
Comparing with the capt_basic_continuous example, this example uses ACOMP
instead of GPIO in measurement period.
CAP polls and samples the touch pad data round by round, in each round, all the
enabled pins are sampled one by one. To monitor one pad sample finished,
both the yes_touch and no_touch interrupt is enabled. The poll done interrupt
is enabled to monitor the poll round finished. The delay period between two
polling round is configurable, in this example, the delay period is set to
about 0.5s.
When the example runs, the sampled data is shown in the terminal, and update
every about 0.5s. When electrode plate is touched, the sampled data changes too.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/capt/capt_acomp_continuous/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/capt/capt_acomp_continuous/example_board_readme.md)
