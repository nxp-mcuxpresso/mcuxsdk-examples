# capt_key

## Overview
This example shows how to use CAPT to detect key touch event.
When the key is touched, LED on the board is on accordingly.
The example first runs the calibration, in this period, all LEDs
are on, please don't touch any key at this time. After all LEDs off
the example starts touch detection, touch any key and the LED turns on.
This example only supports touch one key at the same time.
CAPT sample data are saved in a window for later process.
During calibration period, the variance of each channel is used to judge
whether the channel is stable. Calibration finished only when all channels
are stable. The stable value after calibration is saved as baseline, to
be used in the detection stage.
To remove touch glitch, a software filter is used in this example.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/capt/capt_key/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/capt/capt_key/example_board_readme.md)
