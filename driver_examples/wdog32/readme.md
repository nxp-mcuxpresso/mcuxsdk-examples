# wdog32

## Overview
The WDOG32 Example project is to demonstrate usage of the KSDK wdog32 driver.
In this example, fast testing is first implemented to test the wdog32.
After this, refreshing the watchdog in None-window mode and window mode is executed.
Note wdog32 is disabled in SystemInit function which means wdog32 is disabled
after chip emerges from reset.


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

High Byte test success

----- Refresh test start -----
----- None-window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
Refresh wdog32 3 time
Refresh wdog32 4 time
Refresh wdog32 5 time
Refresh wdog32 6 time
Refresh wdog32 7 time
Refresh wdog32 8 time
Refresh wdog32 9 time
----- Window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
----- Refresh test success  -----

----- End of WDOG32 example  -----

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/driver_examples/wdog32/example_board_readme.md)
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/driver_examples/wdog32/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/driver_examples/wdog32/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/driver_examples/wdog32/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/driver_examples/wdog32/example_board_readme.md)
- [FRDM-KE15Z](../../_boards/frdmke15z/driver_examples/wdog32/example_board_readme.md)
- [FRDM-KE16Z](../../_boards/frdmke16z/driver_examples/wdog32/example_board_readme.md)
- [FRDM-KE17Z](../../_boards/frdmke17z/driver_examples/wdog32/example_board_readme.md)
- [FRDM-KE17Z512](../../_boards/frdmke17z512/driver_examples/wdog32/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/driver_examples/wdog32/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/driver_examples/wdog32/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/driver_examples/wdog32/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/driver_examples/wdog32/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../_boards/mcimx93autoevk/driver_examples/wdog32/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/driver_examples/wdog32/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/driver_examples/wdog32/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/driver_examples/wdog32/example_board_readme.md)
