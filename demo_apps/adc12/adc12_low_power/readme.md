# adc12_low_power

## Overview
The ADC Low Power Demo project is a demonstration program that uses the KSDK software. The
microcontroller is set to a very low power stop (VLPS) mode, and every 500 ms an interrupt wakes up
the ADC module and takes the current temperature sensor value of the microcontroller. While the 
temperature remains within boundaries, both LEDs are on. If the temperature is higher or lower than
average, a led comes off. This demo provides an example to show how ADC works during a VLPS mode and
a simple debugging.

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/demo_apps/adc12_low_power/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/demo_apps/adc12_low_power/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/demo_apps/adc12_low_power/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/demo_apps/adc12_low_power/example_board_readme.md)
