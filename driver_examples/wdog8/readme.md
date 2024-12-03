# wdog8

## Overview
The WDOG8 Example project is to demonstrate usage of the wdog8 driver.
In this example, fast testing is first implemented to test the wdog8.
After this, refreshing the watchdog in None-window mode and window mode is executed.
Note wdog8 is disabled in SystemInit function which means wdog8 is disabled
after chip emerges from reset.


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.
Low Byte test success
High Byte test success

----- Refresh test start -----
----- None-window mode -----
Refresh wdog8 0 time
Refresh wdog8 1 time
Refresh wdog8 2 time
Refresh wdog8 3 time
Refresh wdog8 4 time
Refresh wdog8 5 time
Refresh wdog8 6 time
Refresh wdog8 7 time
Refresh wdog8 8 time
Refresh wdog8 9 time
----- Window mode -----
Refresh wdog8 0 time
Refresh wdog8 1 time
Refresh wdog8 2 time
----- Refresh test success  -----

----- End of WDOG8 example  -----

## Supported Boards
- [FRDM-KE02Z40M](../../_boards/frdmke02z40m/driver_examples/wdog8/example_board_readme.md)
