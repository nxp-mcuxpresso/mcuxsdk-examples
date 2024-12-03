# dsc_cmp_polling

## Overview

The CMP polling Example shows the simplest way to use CMP driver and help user with a quick start.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
CMP's positive channel input. On the negative side, the internal 6-bit DAC is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user's voltage crosses the internal DAC's value. The endless loop in main() function
would detect the logic value of comparator's output, and change the LED. The LED would be turned on when the compare
output is logic one, or turned off when zero.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cmp/polling/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cmp/polling/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cmp/polling/example_board_readme.md)
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/cmp/polling/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/cmp/polling/example_board_readme.md)
