# power_mode_switch_k4

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

## External voltage domain low-power isolation

Isolating an external voltage domain (SPC `EVD_CFG[EVDLPISO]`) reduces low-power current, but it
also resets the I/O pads and analog components in that domain. So a domain must not be isolated
while it hosts the selected wakeup source, or an interface that an external device keeps driving --
typically the debug console. Whether that is the case depends on the target power mode, the wakeup
source, and the board's supply-rail and pin topology, so this demo computes the mask instead of
applying one static constant.

The mask is `class_mask & ~source_mask`, where the board declares both parts in its `app.h`:

| Macro | Meaning |
|---|---|
| `APP_LP_ISO_DOMAINS_SHALLOW` | domains the board may isolate in Sleep and Deep Sleep |
| `APP_LP_ISO_DOMAINS_POWER_DOWN` | domains the board may isolate in Power Down |
| `APP_LP_ISO_DOMAINS_DEEP_POWER_DOWN` | domains the board may isolate in Deep Power Down |
| `APP_LPTMR_WAKEUP_ISO_DOMAINS` | domain LPTMR depends on (usually none -- it is internal) |
| `APP_WAKEUP_BUTTON_WAKEUP_ISO_DOMAINS` | domain the wakeup button's pin depends on |
| `APP_VBAT_WAKEUP_ISO_DOMAINS` | domain the VBAT wakeup pin depends on |

Deeper modes may allow more domains, because fewer interfaces have to stay alive. On KW43 the
reference manual additionally states that `VDD_IO_ABC` should only be set in `EVD_CFG[EVDLPISO]`
when moving to Deep Power Down, which is why the mode class matters and not just the wakeup source.

The resolved mask is deliberately not printed, so that the example's console output stays stable for
automated test patterns. To observe the applied mask, read `SPC0->EVD_CFG[EVDLPISO]` with a debugger
before low-power entry.

Boards that only define the older mode-independent `APP_LPTMR_LOW_POWER_ISO_DOMAINS`,
`APP_WAKEUP_BUTTON_LOW_POWER_ISO_DOMAINS`, and `APP_VBAT_LOW_POWER_ISO_DOMAINS` macros are
unaffected: fallbacks in `power_mode_switch.c` reduce the policy to the legacy per-source constant
for every mode, so their behavior is unchanged.

Those fallbacks reproduce the legacy value only when a source's legacy mask is a subset of the board's
legacy LPTMR mask. That is checked at build time, so a board where it does not hold fails to compile
with a message naming the macro to declare, instead of silently changing behavior. Declare the six
macros above in `app.h` to opt that board into the mode-aware form.

Note on the power manager: `components/power_manager` does not participate in this policy. Its
device layer (for example `components/power_manager/devices/KW43B43ZC7/fsl_pm_device.c`) uses
`SPC_ClearPeriphIOIsolationFlag()` and the power-domain low-power request flags, which are a
separate isolation mechanism from `EVD_CFG`. The mask also depends on the wakeup source the user
picks at the menu, which is application state rather than device state.

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/demo_apps/power_mode_switch/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/demo_apps/power_mode_switch/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/demo_apps/power_mode_switch/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/demo_apps/power_mode_switch/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/demo_apps/power_mode_switch/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/demo_apps/power_mode_switch/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/demo_apps/power_mode_switch/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/demo_apps/power_mode_switch/example_board_readme.md)
- [FRDM-KW43](../../_boards/frdmkw43/demo_apps/power_mode_switch/example_board_readme.md)
