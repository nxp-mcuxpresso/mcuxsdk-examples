# low_power_display_imx8ulp

## Overview
The Low Power display demo application demonstrates how to use CM33 core to
do a tripple buffer pan display with total i.MX8ULP SoC power around 90mW.
In this demo, the display controller, MIPI DSI host is controlled by CM33
instead of Application Domain (A35 cores). The frame buffers are put into the external
PSRAM through FlexSPI interfaces, this can keep the DDR in retention mode to save power.
The display content refresh rate is limited to one fps, RTD domain will enter
sleep mode to save power after updated the display buffer address every seconds.

## Supported Boards
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/demo_apps/low_power_display/example_board_readme.md)
