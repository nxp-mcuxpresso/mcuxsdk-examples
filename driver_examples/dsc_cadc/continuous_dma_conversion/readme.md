# cadc_continuous_dma_conversion

## Overview
The CADC continuous DMA demo application demonstrates the usage of the ADC and DMA peripheral while in a Loop Sequential mode. The
CADC is first set to Loop Sequential mode. In Loop Sequential convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the CADC continues to do conversions on the same CLIST register that initiated the conversion. 
DMA request will be asserted during an CADC conversion READY event noted when any of the ADC_RDY[RDYn] flags is asserted. DMA will transfer
CADC results to memory and if users press any key, demo will average CADC results stored in memory and print average on the terminal.

## Supported Boards
