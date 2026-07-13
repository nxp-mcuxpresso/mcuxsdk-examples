# dpu_1_warp

## Overview

This project shows the arbitrary warping function of fetch warp unit.

The DPU path is configured like this:


```
 ***********************      ************************************************
 *                     *      *                                              *
 *  Coordinate buffer  *      *             Frame buffer                     *
 *                     *      *                                              *
 ***********************      ************************************************
          |                                          |
          v                                          v
 +-----------------+                       +-----------------+
 |                 |                       |                 |
 |    Fetch Eco9   |---------------------->|    FetchRot9    |
 |                 |                       |                 |
 +-----------------+                       +-----------------+
                                                     |
                                           +-----------------+
                                           |                 |
                                           |      Store9     |
                                           |                 |
                                           +-----------------+
                                                     |
                                        ***********************
                                        *                     *
                                        *   output buffer     *
                                        *                     *
                                        ***********************
                                                     |
                                                     |
                                          +-----------------+   +-----------------+
                                          |                 |   |                 |
                                          |   FetchYUV0     |   |  Const Frame 0  |
                                          |                 |   |                 |
                                          +-----------------+   +-----------------+
                                                     |                   |
                                                     |                   v
                                                     |          +-----------------+
                                                     |          |                 |
                                                     +--------->|   LayerBlend 1  |
                                                                |                 |
                                                                +-----------------+
                                                                         |
                                                                         v
                                                                +-----------------+
                                                                |                 |
                                                                |     ExtDst0     |
                                                                |                 |
                                                                +-----------------+
                                                                         |       Content
                                                                         v
                                                                +-----------------+
                                                                |                 |
                                                                |    FrameGen 0   |
                                                                |                 |
                                                                +-----------------+
                                                                         |
                                                                         v
```

## Supported Boards
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/dpu/warp/example_board_readme.md)
- [IMX952-EVK](../../../_boards/imx952evk/driver_examples/dpu/warp/example_board_readme.md)
- [FRDM-IMX95](../../../_boards/frdmimx95/driver_examples/dpu/warp/example_board_readme.md)
- [frdmimx952](../../../_boards/frdmimx952/driver_examples/dpu/warp/example_board_readme.md)
