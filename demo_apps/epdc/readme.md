# epdc

## Overview
This example shows how to use the PXP to process pixels, apply waveform mode
then use the EPDC to drive the E-INK panel.

In this example, first an image of RGB888 pixel format is processed by PXP to generate Y4 pixel data.
Then PXP waveform engine will apply the waveform mode 2 to the pixel data for EPDC to update high quality
image to the screen. The same image is updated to screen 4 times on different locations. Collision shall occur
and non-collided update(s) will appear on screen together within one fresh, the collided update(s) will
wait until the previous update(s) to finish then refresh.
Next the panel is cleared and PXP waveform engine will constantly apply the waveform mode 1 to black dot for
EPDC to draw line on panel to simulate pen input. The update shall be non-flashy and quicker than image update.

## Supported Boards
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/demo_apps/epdc/example_board_readme.md)
