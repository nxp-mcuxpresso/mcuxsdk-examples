# dpu_1_rop

## Overview

This example shows how to use the ROP unit in blit engine. In this project,
there are three input frame buffers to the blit engine. The blit engine
generates two frames using different ROP configurations, so the output frames
are different.

For landscape display panel:

Output frame 0

```
  +------------------+------------------+------------------+
  |                  |                  |                  |
  |   Red            |    Yellow        |      Green       |
  |                  |                  |                  |
  |                  |                  |                  |
  |        +--------------------------------------+        |
  |        |         |                  |         |        |
  |        | Magenta |    White         |  Cyan   |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  +--------|---------+------------------+------------------+
  |        |                                      |        |
  | Black  |                                      | Black  |
  |        |              Blue                    |        |
  |        |                                      |        |
  +--------+--------------------------------------+--------+
```


Output frame 1

```
  +------------------+------------------+------------------+
  |                  |                  |                  |
  |   Cyan           |    Blue          |    Magenta       |
  |                  |                  |                  |
  |                  |                  |                  |
  |        +--------------------------------------+        |
  |        |         |                  |         |        |
  |        |  Green  |    Black         |  Red    |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  +--------|---------+------------------+------------------+
  |        |                                      |        |
  | White  |                                      | White  |
  |        |              Yellow                  |        |
  |        |                                      |        |
  +--------+--------------------------------------+--------+
```

For portrait display panel:

Output frame 0

```
  +------------------+------------------+------------------+
  |                  |                  |                  |
  |   Red            |    Yellow        |      Green       |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |        +--------------------------------------+        |
  |        |         |                  |         |        |
  |        | Magenta |    White         |  Cyan   |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  +--------|---------+------------------+------------------+
  |        |                                      |        |
  | Black  |                                      | Black  |
  |        |              Blue                    |        |
  |        |                                      |        |
  |        |                                      |        |
  |        |                                      |        |
  |        |                                      |        |
  +--------+--------------------------------------+--------+
```


Output frame 1

```
  +------------------+------------------+------------------+
  |                  |                  |                  |
  |   Cyan           |    Blue          |    Magenta       |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |                  |                  |                  |
  |        +--------------------------------------+        |
  |        |         |                  |         |        |
  |        |  Green  |    Black         |  Red    |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  |        |         |                  |         |        |
  +--------|---------+------------------+------------------+
  |        |                                      |        |
  | White  |                                      | White  |
  |        |              Yellow                  |        |
  |        |                                      |        |
  |        |                                      |        |
  |        |                                      |        |
  |        |                                      |        |
  +--------+--------------------------------------+--------+
```

The two frames are shown one by one. Because the switch interval
is controlled by an software loop delay, so the interval might be different
for debug target and release target.

In this project, a new blit engine process is started only when the previous
frame finished.

The DPU path is configured like this:


```
************************   ************************  ************************
*                      *   *                      *  *                      *
* s_redWinFrameBuffer  *   * s_greenWinFrameBuffer*  * s_blueWinFrameBuffer *
*                      *   *                      *  *                      *
************************   ************************  ************************
         |                           |                        |
         v                           v                        v
  +-----------------+       +-----------------+        +-----------------+
  |                 |       |                 |        |                 |
  |  FetchDecode 9  |       |   FetchRot 9    |        |    FetchEco 9   |
  |                 |       |                 |        |                 |
  +-----------------+       +-----------------+        +-----------------+
        |                           |                         |
        |                           v                         |
        |                  +-----------------+                |
        |                  |                 |                |
        +----------------->|      ROP 9      |<---------------+
                           |                 |
                           +-----------------+
                                    |
                                    v
                           +-----------------+
                           |                 |
                           |   BlitBlend 9   |
                           |                 |
                           +-----------------+
                                    |
                                    v
                           +-----------------+
                           |                 |
                           |     Store 9     |
                           |                 |
                           +-----------------+
                                    |
                                    v
                        ************************
                        *                      *
                        * s_displayFrameBuffer *
                        *                      *
                        ************************
                                    |
                                    v
                            +-----------------+   +-----------------+
                            |                 |   |                 |
                            |   FetchYUV0     |   |  Const Frame 0  |
                            |                 |   |                 |
                            +-----------------+   +-----------------+
                                    |                      |
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
                                                  |     ExtDst 0    |
                                                  |                 |
                                                  +-----------------+
                                                           |       Content Stream
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
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/dpu/rop/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/dpu/rop/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/dpu/rop/example_board_readme.md)
