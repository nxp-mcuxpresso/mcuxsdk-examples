# acomp_basic

## Overview

The ACOMP Basic Example shows the simplest way to use ACOMP driver and help user with a quick start.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
ACOMP's negative channel input. On the postive side, the internal voltage ladder is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user's voltage crosses the internal ladder's output. The endless loop in main() function
would detect the logic value of comparator's output, and change the LED. The LED would be turned on when the compare
output is logic one, or turned off when zero.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/acomp/acomp_basic/example_board_readme.md)
- [LPCXpresso802](../../../_boards/lpcxpresso802/driver_examples/acomp/acomp_basic/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/acomp/acomp_basic/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/acomp/acomp_basic/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/acomp/acomp_basic/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/acomp/acomp_basic/example_board_readme.md)
