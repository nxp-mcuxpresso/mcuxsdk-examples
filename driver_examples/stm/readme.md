# stm_match_interrupt

## Overview
The stm driver example is to demonstrate usage of the SDK STM driver with interrupt callback functions. In this example 
two channels are configured with different match values, when STM timer count matches the channel value, "Channel ${number} match." log
will be printed to the terminal from interrupt callback function for this channel.

## Supported Boards
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/driver_examples/stm/example_board_readme.md)