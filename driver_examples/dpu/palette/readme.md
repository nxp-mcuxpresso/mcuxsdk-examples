# dpu_palette

## Overview
This example shows how to use the palette feature of fetch layer and fetch decoder.

The right part is fetch decoder output. The palette is set to blue color varies
from 255 to 0. So the output is blue color with different degrees.

The left part is fetch layer output. In this project, the 8 sublayers are all
enabled, the sublayer 0 output is at the top, and the sublayer 7 output is at
the bottom. All sublayers use the same input framebuffer but use the
different color palettes, so the sublayer output colors are different.
The fetch layer palette entries are divided into 8 groups, one for each sublayer.
Every group has 32 entries, the valid index width is 5. So even though the index
in frame buffer is 8-bit width, only the 5-LSB are used for the palette index.

The DPU path is configured like this:

```
  **********************                    *********************
  *                    *                    *                   *
  *    Frame buffer    *                    *   Frame buffer    *
  *                    *                    *                   *
  **********************                    *********************
          |                                           |
          v                                           v
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
  |                 |   |                 |   |                 |   |                 |
  |   FetchLayer0   |   |  Const Frame 0  |   |   FetchDecode1  |   |  Const Frame 4  |
  |                 |   |                 |   |                 |   |                 |
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
          |                      |                    |                      |
          |                      v                    |                      v
          |             +-----------------+           |             +-----------------+
          |             |                 |           |             |                 |
          +------------>|   LayerBlend 0  |           +------------>|   LayerBlend 1  |
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
