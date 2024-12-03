# power_mode_switch_ll_mcxa

## Overview
The power_mode_switch_ll demo application demonstrates the usage of low level power-related drivers(SPC, CMC, VBAT, WUU, and so on) to enter/exit different power modes.
    - WUU: Help pins and modules generate a temporary wake-up from Power Down or Deep Power Down mode.
    - CMC: Provide the sequencing of the CPU and associated logic through the different modes.
    - SPC: System power controller which is used to configure on-chip regulators in active/low-power modes.

The demo prints the power mode menu through the debug console, where the users can set the MCU to a specific power modes. Users can also select the wake-up source by following the debug console prompts. The purpose of this demo is to show how to switch between different power modes, and how to configure a wake-up source and wakeup the MCU from low power modes.
This demo demonstrates 4 level power modes:
    - Sleep: CPU0 clock is off, and the system clock and bus clock remain ON. Most modules can remain operational.
    - Deep Sleep: Core clock, system clock, and bus clock are all OFF, some modules can remain operational with low power asynchronous clock source(OSC32k or FRO16K which located in VBAT domain) and serve as wake-up sources.
    - Power Down:  Core, system, bus clock are gated off, the VDD_CORE_MAIN and VDD_CORE_WAKE power domains are put into retention mode which means modules in these domains are not operational.
    - Deep Power Down: The whole VDD_CORE domain is power gated. Both CORE_LDO and DCDC are turned off. The device wakes from Deep Power Down mode through the Reset routine.
This demo demonstrates 2 wake-up sources:
    - LPTMR: Located in System power domain, clocked by FRO16K which from VBAT domain. LPTMR is also enabled in WUU domain to wake-up device from power down and deep power down modes.
    - WakeUpButton: The external pin which connected to WUU.

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/demo_apps/power_mode_switch/low_level/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/demo_apps/power_mode_switch/low_level/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/demo_apps/power_mode_switch/low_level/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/demo_apps/power_mode_switch/low_level/example_board_readme.md)
