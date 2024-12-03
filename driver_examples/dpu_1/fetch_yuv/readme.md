# dpu_1_fetch_yuv

## Overview
The DPU Fetch Yuv example shows how to use the DPU driver to enable fetch Yuv
unit and the scaler. The fetch yuv output frame size is smaller than desired
output, so the scaler units are used for the frame size convert.

If this example runs success, rectangles will move in the screen.

The DPU path is configured like this:

                                         **********************
                                         *                    *
                                         *    Frame buffer    *
                                         *                    *
                                         **********************
                                                   |
                                                   v
                                           +-----------------+   +-----------------+
                                           |                 |   |                 |
                                           |   FetchYuv0     |   |  Const Frame 0  |
                                           |                 |   |                 |
                                           +-----------------+   +-----------------+
                                                   |                      |
                                                   v                      |
                                           +-----------------+            |
                                           |                 |            |
                                           |   VScaler 4 &   |            |
                                           |   HScaler 4     |            |
                                           |                 |            |
                                           +-----------------+            |
                                                   |                      v
                                                   |             +-----------------+
                                                   |             |                 |
                                                   +------------>|   LayerBlend 1  |
                                                                 |                 |
                                                                 +-----------------+
                                                                          |
                                                                          v
                                                                 +-----------------+
                                                                 |                 |
                                                                 |    ExtDst 0     |
                                                                 |                 |
                                                                 +-----------------+
                                                                          |
                                                                          v
                                       Content Stream            +-----------------+
                                                                 |                 |
                                                                 |  FrameGen 0     |
                                                                 |                 |
                                                                 +-----------------+
                                                                          |
                                                                          v

## Supported Boards
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/dpu/fetch_yuv/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/dpu/fetch_yuv/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/dpu/fetch_yuv/example_board_readme.md)
