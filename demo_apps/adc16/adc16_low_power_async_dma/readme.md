# adc16_low_power_async_dma

## Overview
The ADC Low Power Async DMA demo application demonstrates the usage of the ADC and DMA peripheral while in a low power mode. The
microcontroller is first set to very low power stop (VLPS) mode. Every 100 ms, low power timer trigger the ADC module convert
value on ADC channel. After 16 times(1,6s) the DMA transfer finish interrupt wake up the CPU to process sampled data, print result to
user and toggle LED.

## Supported Boards
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
- [TWR-KM34Z50MV3](../../../_boards/twrkm34z50mv3/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/demo_apps/adc16_low_power_async_dma/example_board_readme.md)
