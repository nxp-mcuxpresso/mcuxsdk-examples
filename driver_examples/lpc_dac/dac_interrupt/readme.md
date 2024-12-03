# lpc_dac_interrupt

## Overview

The dac_interrupt example shows how to use DAC with interrupts and produce an arbitrary, user-defined waveform of
selectable frequency.The output can be observed with an oscilloscope. 

When the DAC's double-buffer feature is enabled, any write to the CR register will only load the pre-buffer, which
shares its register address with the CR register. The CR itself will be loaded from the pre-buffer whenever the 
counter reaches zero and the interrupt would occur. At the same time the counter is reloaded with the COUNTVAL
register value. In the DAC ISR, user-defined waveform array would be transfered to pre-buffer in order.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/dac/lpc_dac_interrupt/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/dac/lpc_dac_interrupt/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/dac/lpc_dac_interrupt/example_board_readme.md)
