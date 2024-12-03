# dcdc_framework

## Overview
This demo is used to show a kind of usage of DCDC function. In this case,
the DCDC is used as the voltage regulator instead of on-chip LDOs. For the
condition that DCDC input voltage is not so stable, as the battery's voltage
decrease after a few working time, a timer is used to trigger the adjustment
by sampling the ADC value of bandgap/battery input voltage, calculating them
and setting the result into the DCDC module, so that the output of DCDC would
be adjusted and kepted constant as much as possible.
Actually, this use case is usually used in the connectivity application, and
the period of adjust (configured by the timer's timeout period) is also setup
according to requirement.

## Supported Boards
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/demo_apps/dcdc_framework/example_board_readme.md)
