# dpu_fetch_layer

## Overview
The DPU Fetch Layer example shows how to use the DPU driver to display multiple
layers. The DPU unit fetch_layer and fetch_warp support multiple layers, they
are both used in this example.

In this example, the screen is divided to two parts, the left part is for
content stream, fetch_layer is used in this stream. The right part is for
safty stream fetch_warp is used in this stream. In a fetch unit, the sub-layers
could not be alpha blended to another, so when the sub-layers overlay, the layer
with larger sub-layer index is shown.

The DPU unit layer_blend supports alpha blend, this function is enabled in the
content stream. The fetch_layer output is blend to const_frame output.

If this example runs success, rectangles and diamonds will move in the screen.

The DPU path is configured like this:

  **********************                    *********************
  *    Multi-Layer     *                    *   Multi-Layer     *
  *    Frame buffer    *                    *   Frame buffer    *
  *                    *                    *                   *
  **********************                    *********************
          |                                           |
          v                                           v
  +-----------------+   +-----------------+   +-----------------+   +-----------------+
  |                 |   |                 |   |                 |   |                 |
  |   FetchLayer0   |   |  Const Frame 0  |   |   FetchWarp2    |   |  Const Frame 4  |
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
