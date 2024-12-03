# cns_adc_software_trigger

## Overview
The adc_software_trigger example shows how to software trigger ADC conversion.

In this example, ADC resolution is set as 16bit, the reference voltage is selected as the internal 1.8V bandgap, and input
gain is set as 1. So the input voltage range is from 0 to 1.8V. Users can trigger ADC conversion by pressing any key in the
terminal. ADC interrupt will be asserted once the conversion is completed.

The conversion result can be calculated via the following formula:
    Conversion Result = (Vin / Vref) * 32767

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/adc/software_trigger/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/adc/software_trigger/example_board_readme.md)
