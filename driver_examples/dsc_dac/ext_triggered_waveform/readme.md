# dsc_dac_ext_triggered_waveform

## Overview

The dsc_dac_ext_triggered_waveform example shows how to use the DAC module as the waveform generator.

This driver example works in synchronous mode, it means that SYNC_IN signal should be configured to update
the buffered registers. So an external input clock is needed in this demo.
When the users use this driver example to generate the waveform, please provide a 1 kHz external clock signal,
and the data in buffered data register can be refreshed every cycle. Then the sawtooh waveform can be measured
on DAC output pin.

## Supported Boards
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/dac/ext_triggered_waveform/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/dac/ext_triggered_waveform/example_board_readme.md)
