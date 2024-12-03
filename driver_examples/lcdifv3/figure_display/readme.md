# lcdifv3_figure_display

## Overview
The figure display demo is used to show pictures on panel.
Tips:
The picture file(*.png, jpeg...) must be converted to bitmap file(binary file: xxx.bin) by using tool(such as: https://lvgl.io/tools/imageconverter). The bitmap file is loaded by uboot(running on MPU) to memory and a memory flag is set by using uboot command. The flag is used to notify Mcore to update figure.

## Supported Boards
