# pxp_rotate_output

## Overview
The PXP picture rotation project demonstrates how to use the ROTATE0 module to implement
rotation functionality. If this example runs correctly, you will see a square with three 
color(red, green and blue).The rotate degree is changing. In this example the ROTATE0 module 
is used to rotate the image in output buffer, the width and height of the image are swapped 
when the rotation angle is 90 or 270 degree. It is necessary to adjust registers such as 
OUT_LRC and OUT_PS_LRC according to the rotation angle.

## Supported Boards
- [IMX943-EVK](../../../_boards/imx943evk/driver_examples/pxp/rotate_output/example_board_readme.md)
