Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54S018M board
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board

Board settings
==============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.
2. For LPCXpresso54S018M V2.0:JP14 2-3 and JP15 2-3 on.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2. Build the project.
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running
   the demo.


Steps to configure the panels
===============
The project uses RK043FN66HS-CT6 panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h, change LVGL_PANEL to 0 use RK043FN02H-CT.


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.

When Kconfig is used
----------------
Select color depth in "Middleware->LVGL Configuration -> Color Settings -> Color depth",
16-bit, 8-bit, and 1-bit are supported.

When package is used
----------------
Modify the mcux_config.h change `CONFIG_LV_COLOR_DEPTH` to 16, 8, or 1.


Running the demo
================
This demo is an external flash plain load demo, ROM will copy image in external flash to SRAMX to run:
1. Build the demo to generate a bin file.
   Note: If need to generate bin file using MCUXpresso IDE, below steps need to be followed:
         Set in example Properties->C/C++ Build->Settings->Build steps->Post-build steps->Edit
         enbable arm-none-eabi-objcopy -v -O binary "&{BuildArtifactFileName}" "&{BuildArtifactFileBaseName}.bin"

         This plainload example linked the vector table to 0x00000000, but program to external flash 0x10000000.

2. Program the bin file to external on board flash via SEGGER J-FLASH Lite(V6.22 or higher):

   a. Open SEGGER J-FLASH Lite, select device LPC54S018M.

   b. Click the 'Erase Chip' to erase the extrenal flash.(if can not success, press SW4 button and reset the board, and try to erase again)

   c. Select the bin data file, set the '.bin/Erase Start' address to 0x10000000, then click 'Program Device'
Note: Please use above way to program the binary file built by armgcc tool chain to external flash.
      For IAR, KEIL, MCUXpresso IDE, you can use the IDE tool to program the external flash.

If this example runs correctly, the sample GUI is displayed.

This board also supports LVGL 1-bit, 8-bit color depth, to use this feature, modify
LV_COLOR_DEPTH as 1 or 8 in lv_conf.h.
