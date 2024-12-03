# dsc_dac_pit_sync_sawtooth_waveform

## Overview

The dac_pit_sync_sawtooth_waveform demo sets the DAC module as Automatic mode to generate one shot sawtooth waveform.
The SYNC signal is selected as external SYNC_IN signal(PIT sync out signal connected by XBAR) and the frequency of extern
SYNC_IN signal is 1Hz. The frequency of the REFRESH signal is set as fbus/500. At the falling edge of PIT sync out signal,
the maximum value of the be updated, and the new waveform will be generated. The DAC output pin will create a single
sawtooth waveform and stop at the final value. It will remain at this final value until a new falling edge occurs on the
pit sync out signal, and the new sawtooth waveform will be generated.

## Supported Boards
- [MC56F81000-EVK](../../../../_boards/mc56f81000evk/demo_apps/dac_external_sync/pit_sync_sawtooth_waveform/example_board_readme.md)
- [MC56F83000-EVK](../../../../_boards/mc56f83000evk/demo_apps/dac_external_sync/pit_sync_sawtooth_waveform/example_board_readme.md)
