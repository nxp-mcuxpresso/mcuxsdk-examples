# isi_dwc_mipi_csi2_yuv422

## Overview
This example shows how to use ISI to receive camera frame through MIPI CSI2.

In this example, the camera input frame is YUV422 format and ISI output frame
is RGB565 format. The output frame is shown in the screen.

NOTE:
1. the example is for newly Synopsys DWC MIPI CSI2 driver and IMX95 platform.
2. This example must run continously to get the right result, breaking during debug
will results to display tearing. Because the ISI driver maintains IP state through
ISR, if the core is halted, then driver state does not match the IP state. When
exiting debug, the core might be halted for a short while, so the tearing may
happen when exiting debug.

If the camera and LCD panel resolutions are not the same, then only part of the
camera frame will be shown.

## Supported Boards
- [IMX95LP4XEVK-15](../../../../_boards/imx95lp4xevk15/driver_examples/isi/dwc_mipi_csi2/yuv422/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../../_boards/imx95lpd5evk19/driver_examples/isi/dwc_mipi_csi2/yuv422/example_board_readme.md)
- [imx95verdinevk](../../../../_boards/imx95verdinevk/driver_examples/isi/dwc_mipi_csi2/yuv422/example_board_readme.md)
