# lpc_dac_dma

## Overview

The dac_dma example shows how to use DAC with dma and produce an arbitrary, user-defined waveform of
selectable frequency.The output can be observed with an oscilloscope. 

When the DAC's double-buffer feature is enabled, any write to the CR register will only load the pre-buffer, which
shares its register address with the CR register. The CR itself will be loaded from the pre-buffer whenever the 
counter reaches zero and the DMA request would be raised. At the same time the counter is reloaded with the COUNTVAL
register value. user-defined waveform array would be transfered to pre-buffer in order by DMA.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/dac/lpc_dac_dma/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/dac/lpc_dac_dma/example_board_readme.md)
