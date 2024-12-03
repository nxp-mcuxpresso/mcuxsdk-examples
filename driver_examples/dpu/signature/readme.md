# dpu_signature

## Overview

This project shows how to use the DPU signature unit. It shows three rectangles,
the blue one and yellow one indicate two signature evaluation windows. The red
rectangle is moving in the screen, when the red rectangle moved into the regions
of evaluation window, the window's measured CRC is different with the reference
value, then panic occurs, the window turns to black accordingly.
