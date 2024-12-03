# mcx_opamp_dac_adc

## Overview

The OPAMP DAC ADC example demonstrates how to use the OPAMP PGA mode. 
In this example, both the positive and negative inputs should be connected 
to GND, the positive reference voltage is set to the DAC output, and the OPAMP
output is 1X the DAC output. When the DAC output changes, the OPAMP changes accordingly.

In PGA mode, the OPAMP output pin does not support direct measurement. Only the ADC is 
supported to sample the OPAMP output voltage, and the output range of the OPAMP positive
reference voltage is 0 to VDDA-0.8V.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/opamp/opamp_dac_adc/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/opamp/opamp_dac_adc/example_board_readme.md)
