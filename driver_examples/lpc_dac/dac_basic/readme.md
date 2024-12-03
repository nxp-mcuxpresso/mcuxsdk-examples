# lpc_dac_basic

## Overview

The dac_basic example shows how to use DAC module simply as the general DAC converter.

When the DAC's double-buffer feature is not enabled, the CR register is used as the DAC output data register directly.
The converter would always output the value of the CR register. In this example, it gets the value from terminal,
outputs the DAC output voltage through DAC output pin.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/dac/lpc_dac_basic/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/dac/lpc_dac_basic/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/dac/lpc_dac_basic/example_board_readme.md)
