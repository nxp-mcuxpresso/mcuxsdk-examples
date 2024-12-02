# mcglite

## Overview
The MCG_Lite example shows how to use MCG_Lite driver:

 1. How to use the mode functions for MCG_Lite mode switch.
 2. How to use the frequency functions to get current MCG_Lite frequency.
 3. Work flow
  Reset mode --> LIRC8M
    LIRC8M --> HIRC
    HIRC   --> LIRC2M
    LIRC2M --> EXT
    EXT    --> HIRC
    HIRC   --> LIRC8M
    LIRC8M --> EXT
    EXT    --> LIRC2M
    LIRC2M --> HIRC
    HIRC   --> EXT
    EXT    --> LIRC8M
    LIRC8M --> LIRC2M
    LIRC2M --> LIRC8M
In this example, because the debug console's clock frequency may change,
so the example running information is not output from debug console. Here the
LED blinks to show that the example finished successfully.

## Supported Boards
- [FRDM-K32L2B](../../_boards/frdmk32l2b/driver_examples/mcglite/example_board_readme.md)
- [FRDM-MCXC041](../../_boards/frdmmcxc041/driver_examples/mcglite/example_board_readme.md)
- [FRDM-MCXC242](../../_boards/frdmmcxc242/driver_examples/mcglite/example_board_readme.md)
- [FRDM-MCXC444](../../_boards/frdmmcxc444/driver_examples/mcglite/example_board_readme.md)
