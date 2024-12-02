# adc16_low_power

## Overview
The ADC Low Power demo application demonstrates the usage of the ADC peripheral while in a low power mode. The
microcontroller is first set to very low power stop (VLPS) mode. Every 500 ms, an interrupt wakes up the ADC module and
reads the current temperature of the microcontroller. While the temperature remains within boundaries, both LEDs are on.
If the core temperature is higher or lower than average, the LEDs change state respectively.

## Supported Boards
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/demo_apps/adc16_low_power/example_board_readme.md)
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/demo_apps/adc16_low_power/example_board_readme.md)
- [FRDM-MCXC041](../../../_boards/frdmmcxc041/demo_apps/adc16_low_power/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/demo_apps/adc16_low_power/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/demo_apps/adc16_low_power/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/demo_apps/adc16_low_power/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/demo_apps/adc16_low_power/example_board_readme.md)
