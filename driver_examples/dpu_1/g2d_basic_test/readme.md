# g2d_basic_test

## Overview
This project is for i.MX95 M7 and tests G2D operations using the Blit Engine.
The Blit Engine is controlled using the G2D library API. g2d_basic_test.c creates pixel buffers and specifies the type 
of operation to be performed. The API sends the requested operation to the Blit Engine, which process the pixels
and places the result in the destination buffer. For each test, the destination buffer is checked for correctness.

## Running the demo
1. $ west build -b imx95lpd5evk19 examples/driver_examples/dpu/g2d_basic_test/ -Dcore_id=cm7
2. copy the image (ex: /mcuxsdk/build/m7_image.bin) to your image builder (ex: /<bsp-dir>/imx-mkimage/iMX95)
3. $ make SOC=iMX95 flash_m7 LPDDR_TYPE=lpddr5 OEI=YES
4. program the board with the created flash.bin (ex: $ uuu.exe flash.bin)

## Supported Boards
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/driver_examples/dpu/g2d_basic_test/example_board_readme.md)
