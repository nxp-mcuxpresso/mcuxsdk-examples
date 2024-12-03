# power_manager_lpc

## Overview
The power_manager_lpc application shows the usage of normal power mode control APIs for entering the three kinds of
low power mode: Sleep mode, Deep Sleep mode and Sleep Power Down mode. When the application runs to each low power
mode, the device would cut off the power for specific modules to save energy. The device can be also waken up by
prepared wakeup source from external event.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary modules during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, there would be current leakage on the pin, which will increase the power consumption.
 - Other low power consideration based on the actual application hardware.
 - In order to meet typedef power consumption of DateSheet manual, Please configure MCU under the following conditions.
     • Configure all pins as GPIO with pull-up resistor disabled in the IOCON block.
     • Configure GPIO pins as outputs using the GPIO DIR register.
     • Write 1 to the GPIO CLR register to drive the outputs LOW.
     • All peripherals disabled.

## Supported Boards
- [LPCXpresso51U68](../../_boards/lpcxpresso51u68/demo_apps/power_manager_lpc/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/demo_apps/power_manager_lpc/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/demo_apps/power_manager_lpc/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/demo_apps/power_manager_lpc/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/demo_apps/power_manager_lpc/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/demo_apps/power_manager_lpc/example_board_readme.md)
