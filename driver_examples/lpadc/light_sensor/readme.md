# lpadc_light_sensor

## Overview
The lpadc_light_sensor example shows how to use the LPADC driver to measure the voltage value
in the circuit with a light sensor. In this example, the specific ADC channel has been connected
into the circuit by board design. When running the project, typing any key into the debug console
would trigger the conversion. The execution would check the FIFO valid flag in a loop until the flag
is asserted, which means the conversion is completed. Then read the conversion result value and print
it to the debug console.

## Supported Boards
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpadc/light_sensor/example_board_readme.md)
- [FRDM-MCXW72](../../../_boards/frdmmcxw72/driver_examples/lpadc/light_sensor/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpadc/light_sensor/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpadc/light_sensor/example_board_readme.md)
