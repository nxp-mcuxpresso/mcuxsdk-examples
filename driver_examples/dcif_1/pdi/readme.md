# dcif_pdi

## Overview
This example demonstrates the DCIF PDI (parallel data interface) low latency
camera preview path:

    OV5640 -> MIPI CSI2 RX -> ISI (scaling, RGB888) -> pixel-link
           -> REFORMATTER (PL_TO_PDI) -> DCIF PDI input (layer 0)
           -> DCIF DPI output -> panel

Unlike the memory based camera display path, the camera pixel stream reaches the
display directly through the pixel-link and REFORMATTER, bypassing the frame
buffer copy, for lower latency and less memory requirement.

NOTE:
The PDI usage has restrictions:
    Only works for DCIF DPI output interface.
    The layer input size must be the same as the DPI output size.
    The camera input frame rate must be the same as the DCIF's DPI output rate.

## Supported Boards
- MIMXRT2660-EVK
