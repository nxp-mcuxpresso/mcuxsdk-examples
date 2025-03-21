# power_mode_switch_mcxe

## Overview

The Power mode switch demo application demonstrates the use of power modes in the MCUXpresso SDK. The demo prints the power mode menu through the debug console where the users can set the MCU to a specific power mode. The user can also select the wakeup source by following the debug console prompts. The purpose of this demo is to show how to switch between different power modes and how to configure a wakeup source to wakeup the MCU from low power modes.

Tips:
This demo aims to show how the various power mode can switch to each other. In actual low power use case, to save energy and reduce the consumption even more, many things can be done including:

- Disable the clock for unnecessary modules during low power mode. That means programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
- Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux and so on.
- Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, there would be current leakage on the pin which will increase the power consumption.
- Other low power consideration based on the actual application hardware.
- Debug pins(e.g SWD_DIO) would consume additional power, the application should disable related pins or disconnect them.

## Supported Boards

- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/demo_apps/power_mode_switch/low_level/example_board_readme.md)