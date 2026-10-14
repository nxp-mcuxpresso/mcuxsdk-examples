# card_freertos

## Overview

This example shows how to use the FlexGPU component. It composes a playing-card
style graphic (a "7 of diamonds") into an off-screen buffer. Generated ARGB2222
assets (a digit "7" and diamond pips) are drawn onto a small card buffer, which
is then perspective-blit onto a larger main buffer. The frame is rendered once
from a FreeRTOS task.

## Supported Boards
- [MIMXRT2660-EVK](../../_boards/mimxrt2660evk/vglite_examples/card_freertos/example_board_readme.md)
