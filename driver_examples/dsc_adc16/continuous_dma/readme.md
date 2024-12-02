# dsc_adc16_continuous_dma

## Overview
The ADC16 continuous DMA demo application demonstrates the usage of the ADC and DMA peripheral while in a continuous mode. The
ADC16 is first set to continuous mode. In continuous convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the ADC16 continues to do conversions on the same SCn register that initiated the conversion. 
DMA request will be asserted during an ADC16 conversion complete event noted when any of the SC1n[COCO] flags is asserted. DMA will transfer
ADC16 results to memory and if users press any key, demo will average ADC16 results stored in memory and print average on the terminal.

## Supported Boards
