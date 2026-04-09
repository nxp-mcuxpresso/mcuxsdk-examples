# tpm_combine_pwm

## Overview
The TPM project is a demonstration program of generating a combined PWM signal by the SDK TPM driver. It sets up the TPM
hardware block to output PWM signals(24kHZ) on two TPM channels. The example also show's the complementary mode of operation
and deadtime insertion. The PWM duty cycle is manually updated and is initially 10%.
On boards that have 2 LEDs connected to the TPM pins, the user will see
a change in LED brightness if user enter different values.
And if the board do not support LEDs to show, the outputs can be observed by oscilloscope.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [IMX952LPD5EVK-19](../../../_boards/imx952evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [IMX952LPD5EVK-15](../../../_boards/imx952evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCX-W71-EVK](../../../_boards/mcxw71evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-IMX95](../../../_boards/frdmimx95/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-IMX95-PRO](../../../_boards/frdmimx95pro/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCIMX93W-EVK](../../../_boards/mcimx93wevk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [KW47-LOC](../../../_boards/kw47loc/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [MCXW72-LOC](../../../_boards/mcxw72loc/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-MCXW72](../../../_boards/frdmmcxw72/driver_examples/tpm/combine_pwm/example_board_readme.md)
- [FRDM-KW43](../../../_boards/frdmkw43/driver_examples/tpm/combine_pwm/example_board_readme.md)
