# dpu_fetch_decode

## Overview
The DPU Fetch Decode example shows how to use the DPU driver to enable fetch decoder
unit and the scaler. The fetch decoder output frame size is smaller than desired
output, so the scaler units are used for the frame size convert.

In this example, the screen is divided to two parts, one for content stream, and
the other for safty stream.

If this example runs success, rectangles will move in the screen.

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
  |   FetchDecode0  |   |  Const Frame 0  |   |   FetchDecode1  |   |  Const Frame 4  |
  |                 |   |                 |   |                 |   |                 |
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
          |                      |                    |                      |
          v                      |                    v                      |
  +-----------------+            |            +-----------------+            |
  |                 |            |            |                 |            |
  |   VScaler 4 &   |            |            |   VScaler 5 &   |            |
  |   HScaler 4     |            |            |   HScaler 5     |            |
  |                 |            |            |                 |            |
  +-----------------+            |            +-----------------+            |
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
