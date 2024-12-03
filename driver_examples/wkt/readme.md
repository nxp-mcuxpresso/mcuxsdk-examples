# wkt_example

## Overview
The WKT project is a simple demonstration program of the SDK WKT driver. It sets up the WKT hardware 
block to trigger a periodic interrupt after loading a counter value and counting down to 0. 
When the WKT interrupt is triggered a message printed on the UART terminal and the LED is toggled on the board.

Depending on the clock source, the WKT can be used for waking up the part from any low power mode or for general-purpose
timing.

## Supported Boards
- [LPC845BREAKOUT](../../_boards/lpc845breakout/driver_examples/wkt/example_board_readme.md)
- [LPCXpresso802](../../_boards/lpcxpresso802/driver_examples/wkt/example_board_readme.md)
- [LPCXpresso804](../../_boards/lpcxpresso804/driver_examples/wkt/example_board_readme.md)
- [LPCXpresso824MAX](../../_boards/lpcxpresso824max/driver_examples/wkt/example_board_readme.md)
- [LPCXpresso845MAX](../../_boards/lpcxpresso845max/driver_examples/wkt/example_board_readme.md)
- [LPCXpresso860MAX](../../_boards/lpcxpresso860max/driver_examples/wkt/example_board_readme.md)
