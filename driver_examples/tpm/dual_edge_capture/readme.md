# tpm_dual_edge_capture

## Overview
The TPM project is a demonstration program of the SDK TPM driver's dual-edge capture feature.
This feature is available only on certain SoC's.
The example sets up a TPM channel-pair for dual-edge capture. Meanwhile, the capture mode is set up one-shot, 
using API TPM_SetupDualEdgeCapture(). Once the input signal is received and the second edge is detected,
the interrupt flag will be cleared and the interrupt overflow will be disabled.
The example gets the capture value of the input signal using API TPM_GetChannelValue().
This example will print the capture values and period of the input signal on the terminal window.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/tpm/dual_edge_capture/example_board_readme.md)
