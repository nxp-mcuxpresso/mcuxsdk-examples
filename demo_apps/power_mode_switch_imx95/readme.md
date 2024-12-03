# power_mode_switch_rtos_imx95

## Overview
The Power mode switch demo application demonstrates the use of power modes in the KSDK. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. The user can also set the wakeup
source by following the debug console prompts. The purpose of this demo is to show how to switch between different power
 modes, and how to configure a wakeup source and wakeup the MCU from low power modes.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume addtional power, had better to disable related pins or disconnect them.

## Supported Boards
- [IMX95LP4XEVK-15](../../_boards/imx95lp4xevk15/demo_apps/power_mode_switch/example_board_readme.md)
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/demo_apps/power_mode_switch/example_board_readme.md)
- [imx95verdinevk](../../_boards/imx95verdinevk/demo_apps/power_mode_switch/example_board_readme.md)
