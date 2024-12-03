# mcx_opamp_dac_lpadc

## Overview

The OPAMP DAC LPADC example demonstrates how to use the OPAMP driver. In this example, 
the OUTSW, BUFFEN, ADCSW1, and ADCSW2 are enabled, and the user can use the ADC to measure the
OPAMPx_INT voltage and the OPAMPx_OBS voltage. The user needs to connect the OPAMP positive
input port and negative input port to GND. The positive (OPAMPx_INP0) reference voltage is
set to the DAC output, the user selects the OPAMP negative gain by using the PC keyboard, and the
positive gain is fixed to 1x, So, the OPAMP output voltage is equal to ((1 + Ngain) / 2)Vpref.

Please note that the user needs to ensure that the serial port terminal used is CR instead of LF
or CR+LF when pressing the enter key.

## Supported Boards
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/opamp/opamp_dac_lpadc/example_board_readme.md)
