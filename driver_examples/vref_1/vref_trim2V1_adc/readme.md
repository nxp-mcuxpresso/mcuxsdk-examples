# vref_1_example

## Overview
The vref example shows how to use the vref driver.

In this example, the adc16 module is initiealized and used to measure the VREF output voltage. So, it cannot use interal
VREF as the reference voltage. Instead, it can use VDD_ANA or VREFH, for detailed information, please refer to device
datasheet.

Then, user should configure the VREF output pin as the ADC16's sample input. When running the project, it will measure
the VREF output voltage under different trim value.

Note: This example is only used for platforms that have the UTRIM[TRIM2V1] function.

## Supported Boards
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/vref_1/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/vref_1/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/vref_1/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/vref_1/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/vref_1/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/vref_1/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/vref_1/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/vref_1/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/vref_1/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/vref_1/example_board_readme.md)
