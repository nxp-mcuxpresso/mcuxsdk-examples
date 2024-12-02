# dpu_warp

## Overview

This project shows the arbitrary warping function of fetch warp unit. Two planes
are shown, the right part is a chess board before warping, the left part is the
chess board after warping.

The DPU path is configured like this:


 ***********************      ************************************************
 *                     *      *                                              *
 *  Coordinate buffer  *      *             Frame buffer                     *
 *                     *      *                                              *
 ***********************      ************************************************
          |                    |                                           |
          v                    v                                           v
 +-----------------+   +-----------------+   +-----------------+   +-----------------+   +-----------------+
 |                 |   |                 |   |                 |   |                 |   |                 |
 |   FetchWarp2    |-->|   FetchWarp2    |   |  Const Frame 0  |   |   FetchDecode1  |   |  Const Frame 4  |
 |                 |   |                 |   |                 |   |                 |   |                 |
 +-----------------+   +-----------------+   +-----------------+   +-----------------+   +-----------------+
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
