# dpu_rop

## Overview

This example shows how to use the ROP unit in blit engine. In this project,
there are three input frame buffers to the blit engine. The blit engine
generates two frames using different ROP configurations, so the output frames
are different.

For landscape display panel:

Output frame 0

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


Output frame 1

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

For portrait display panel:

Output frame 0

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


Output frame 1

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

The two frames are shown one by one. Because the switch interval
is controlled by an software loop delay, so the interval might be different
for debug target and release target.

In this project, a new blit engine process is started only when the previous
frame finished.

The DPU path is configured like this:


************************   ************************  ************************
*                      *   *                      *  *                      *
* s_redWinFrameBuffer  *   * s_greenWinFrameBuffer*  * s_blueWinFrameBuffer *
*                      *   *                      *  *                      *
************************   ************************  ************************
         |                           |                        |
         v                           v                        v
  +-----------------+       +-----------------+        +-----------------+
  |                 |       |                 |        |                 |
  |  FetchDecode 9  |       |   FetchWarp 9   |        |    FetchEco 9   |
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
                            |   FetchDecode1  |   |  Const Frame 4  |
                            |                 |   |                 |
                            +-----------------+   +-----------------+
                                    |                      |
                                    |                      v
                                    |             +-----------------+
                                    |             |                 |
                                    +------------>|   LayerBlend 2  |
                                                  |                 |
                                                  +-----------------+
                                                           |
                                                           v
                                                  +-----------------+
                                                  |                 |
                                                  |  ExtDst 4 or 5  |
                                                  |                 |
                                                  +-----------------+
                                                           |       Safety stream
                                                           v
                                                  +-----------------+
                                                  |                 |
                                                  | FrameGen 0 or 1 |
                                                  |                 |
                                                  +-----------------+
                                                           |
                                                           v
