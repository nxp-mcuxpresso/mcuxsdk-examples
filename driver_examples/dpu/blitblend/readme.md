# dpu_blitblend

## Overview

In this project, the safety stream and content stream are both displayed.
safety stream on the left part, content stream on the right part.
The blit engine generates the frame buffers, the blend modes are different
for safety stream and content stream.

For each stream, there are 5 rectangles, 4 of them are fixed (red, green, blue,
and white) and 1 of them is floating (yellow). When the floating rectangle
overlay occurs, the blend modes are different for safety stream and content
stream. For the left part, the overlay color is fixed rectangle adds floating
rectangle. For the right part, the overlay color is fixed rectangle subtracts
the floating rectangle.

In this project, the blit engine shadow register feature is used. New
configuration is written to the registers during the previous process is
still on-going. The pipline sequence completed interrupt is used to make sure
all pending process finished when necessary.

The DPU path is configured like this:

```
****************************   ******************************
* s_redWinFrameBuffer      *   *                            *
* s_greenWinFrameBuffer    *   *    s_floatWinFrameBuffer   *
* s_blueWinFrameBuffer     *   *                            *
* s_whiteWinFrameBuffer    *   *                            *
****************************   ******************************
         |                             |
         v                             v
 +---------------+             +---------------+
 |               |             |               |
 |  FetchWarp 9  |             | FetchDecode 9 |
 |               |             |               |
 +---------------+             +---------------+
         |                             |
         |                             v
         |                     +---------------+
         |                     |               |
         |                     |     ROP 9     |
         |                     |               |
         |                     +---------------+
         |                             |
         |                             v
         |                     +---------------+
         |                     |               |
         +-------------------->|  BlitBlend 9  |
                               |               |
                               +---------------+
                                       |
                                       v
                               +---------------+
                               |               |
            +------------------|    Store 9    |-------------+
            |                  |               |             |
            |                  +---------------+             |
            v                                                v
******************************         ******************************
*                            *         *                            *
* s_displayFrameBuffer[2, 3] *         * s_displayFrameBuffer[0, 1] *
*                            *         *                            *
******************************         ******************************
          |                                           |
          v                                           v
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
  |                 |   |                 |   |                 |   |                 |
  |   FetchDecode0  |   |  Const Frame 0  |   |   FetchDecode1  |   |  Const Frame 4  |
  |                 |   |                 |   |                 |   |                 |
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
          |                      |                    |                      |
          |                      v                    |                      v
          |             +-----------------+           |             +-----------------+
          |             |                 |           |             |                 |
          +------------>|   LayerBlend 0  |           +------------>|   LayerBlend 2  |
                        |                 |                         |                 |
                        +-----------------+                         +-----------------+
                                 |                                           |
                                 v                                           v
                        +-----------------+                         +-----------------+
                        |                 |                         |                 |
                        |  ExtDst 0 or 1  |                         |  ExtDst 4 or 5  |
                        |                 |                         |                 |
                        +-----------------+                         +-----------------+
                                 |                                           |       Safety stream
                                 |                                           v
        Content Stream           |                                  +-----------------+
                                 |                                  |                 |
                                 +--------------------------------->| FrameGen 0 or 1 |
                                                                    |                 |
                                                                    +-----------------+
                                                                             |
                                                                             v
```
