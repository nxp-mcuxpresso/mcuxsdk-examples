# dsc_cop

## Overview
The COP Example project is to demonstrate usage of the cop driver.In
this example, after 5 times of refreshing in interrupt routine, interrupt will
be disabled, thus the COP is not serviced, then a timeout reset is generated.
Please notice that Write Protect bit in CTRL register are write-once only, it can be cleared only by resetting COP module.
All COP registers are read-only when Write Protect bit set.

## Supported Boards
- [MC56F80000-EVK](../../_boards/mc56f80000evk/driver_examples/cop/example_board_readme.md)
- [MC56F81000-EVK](../../_boards/mc56f81000evk/driver_examples/cop/example_board_readme.md)
- [MC56F83000-EVK](../../_boards/mc56f83000evk/driver_examples/cop/example_board_readme.md)
- [TWR-MC56F8200](../../_boards/twrmc56f8200/driver_examples/cop/example_board_readme.md)
- [TWR-MC56F8400](../../_boards/twrmc56f8400/driver_examples/cop/example_board_readme.md)
