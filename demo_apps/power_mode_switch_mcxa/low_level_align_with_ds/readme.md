# power_mode_switch_ll_mcxa

## Overview
The power_mode_switch_ll demo application demonstrates the usage of low level power-related drivers(SPC, CMC, VBAT, WUU, and so on)
to enter/exit different power modes.
    - WUU: Help pins and modules generate a temporary wake-up from Power Down or Deep Power Down mode.
    - CMC: Provide the sequencing of the CPU and associated logic through the different modes.
    - SPC: System power controller which is used to configure on-chip regulators in active/low-power modes.

The demo prints the power mode menu through the debug console, where the users can set the MCU to a specific power modes.
Users can also select the wake-up source by following the debug console prompts. The purpose of this demo is to show how to
switch between different power modes, and how to configure a wake-up source and wakeup the MCU from low power modes.

This demo demonstrates 4 level power modes:
    - Sleep: CPU clock is off, and the system clock and bus clock remain on. Most modules can remain operational.
    - Deep Sleep: Core, system, and bus clock are all off, some modules can remain operational with low power asynchronous clock source
                  and serve as wake-up sources.
    - Power Down: Core, system, and bus clock are all off, the CORE power domain is put into retention mode which means modules in these
                  domains are not operational.
    - Deep Power Down: The whole CORE domain is power gated. Both LDO_CORE is turn off. The device wakes from Deep Power Down mode
                       through the Reset routine.
This demo demonstrates 2 wake-up sources:
    - Timer: Located in System power domain, clocked by FRO16K which from VBAT. The timer is also enabled in WUU to wake-up device from power down
             and deep power down modes.
    - Button: The external pin which connected to WUU.

## Supported Boards
- [FRDM-MCXA577](../../../_boards/frdmmcxa577/demo_apps/power_mode_switch/low_level/example_board_readme.md)
- [FRDM-MCXA287](../../../_boards/frdmmcxa287/demo_apps/power_mode_switch/low_level/example_board_readme.md)
