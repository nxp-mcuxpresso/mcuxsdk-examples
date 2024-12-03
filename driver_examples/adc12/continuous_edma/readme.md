# adc12_continuous_edma

## Overview
The ADC12 continuous EDMA demo application demonstrates the usage of the ADC and EDMA peripheral while in a continuous mode. The
ADC12 is first set to continuous mode. In continuous convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the ADC12 continues to do conversions on the same SCn register that initiated the conversion. 
EDMA request will be asserted during an ADC12 conversion complete event noted when any of the SC1n[COCO] flags is asserted. EDMA will transfer
ADC12 results to memory and if users press any key, demo will average ADC12 results stored in memory and print average on the terminal.

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/adc12/continuous_edma/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/adc12/continuous_edma/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/adc12/continuous_edma/example_board_readme.md)
