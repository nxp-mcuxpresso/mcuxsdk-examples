# dsc_dac_waveform_generator

## Overview

The dsc_dac waveform_generator example shows how to use the DAC module as the waveform generator.

This driver example works in asynchronous mode, it means that SYNC_IN signal is not needed to update
the buffered registers.
When the users use this driver example to generate the desired waveform, please confirm the frequency
of the desired waveform is justifiable(For the small frequency of the desired waveform may lead to the compare value being out of range).
Please also confirm the step size is reasonable based on the settling time of the DAC module.

## Supported Boards
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/dac/waveform_generator/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/dac/waveform_generator/example_board_readme.md)
