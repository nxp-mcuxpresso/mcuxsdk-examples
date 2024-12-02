# dpu_character

## Overview
The DPU Character example shows how to use the DPU driver to show
characters. In this example, the fetch decode is used to fetch frame buffer
to show. Two frame buffers are used, CPU fills the inactive frame buffer
DPU shows the active frame buffer. When the example runs, you can see
character changing in the middle of the screen.

Please note that in this example the software loop is used for delay, so
the character change frequency might be different for debug target and
release target.

The DPU path is configured like this:

                                     *********************
                                     *                   *
                                     *   Frame buffer    *
                                     *                   *
                                     *********************
                                               |
                                               v
                 +-----------------+   +-----------------+   +-----------------+
                 |                 |   |                 |   |                 |
                 |  Const Frame 0  |   |   FetchDecode1  |   |  Const Frame 4  |
                 |                 |   |                 |   |                 |
                 +-----------------+   +-----------------+   +-----------------+
                          |                    |                      |
                          |                    |                      |
                          |                    |                      |
                          |                    |                      v
                          |                    |             +-----------------+
                          |                    |             |                 |
                          |                    +------------>|   LayerBlend 2  |
                          |                                  |                 |
                          |                                  +-----------------+
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
