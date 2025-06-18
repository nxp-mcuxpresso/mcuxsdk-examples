# dpu_1_g2d_basic_test

## Overview
This is a test of the graphics performance of the Blit Engine hardware and the graphics enablement
of the G2D (2D graphics) library.

g2d_basic_test.c uses the G2D API to send graphics operations to
the Blit Engine. The engine does pixel processing and places its result in the destination buffer.
The engine operations are timed and checked for correctness.


```
__________________________                         __________________________
|                        |-----(G2D operation)---> |                        |
|  main program:         |                         |  Blit Engine:          |
| - G2D API calls        |                         | - process pixels       |
| - check performance    |                         | - store the result     |
| - check correctness    |                         |                        |
|________________________| <---(result frame)------|________________________|
```

## Supported Boards
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/driver_examples/dpu/g2d_basic_test/example_board_readme.md)
