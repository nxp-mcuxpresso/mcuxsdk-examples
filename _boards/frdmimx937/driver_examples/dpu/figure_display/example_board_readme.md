Hardware requirements
=====================
- Micro USB cable
- FRDMIMX937 board
- J-Link Debug Probe
- USB power supply
- Personal Computer
- WAVESHARE_DSI2DPI panel

Board settings
==============
For LVDS2HDMI card(1920*1080), connect HDMI port J16 to panel.
Ensure the J15 jumper connect to 12V, J13 jumper connect to 3.3V
For WAVESHARE_DSI2DPI panel(1024*600), connect MIPI DSI port J12 to panel
Use Dupont wires to connect the following pins.
 ——————————————————————————————————
| panel FAN 5V ----------- J18-2  |
| panel FAN GND ---------- J18-39 |
| panel FAN SCL1 --------- J2-7   |
| panel FAN SDA1 --------- J2-9   |
 —————————————————————————————————

Prepare the Demo
================
1.  Connect USB power supply to the board.
2.  Connect a micro USB cable between the host PC and the J1 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Load the figure binary file by using Jlink command "loadbin xxx.bin 0x80000000" and "loadbin xxx.bin 0x807e9000"(default LVDS2HDMI card).

   -----------------------------------------------------------------
   | panel name        | buffer address 1 |   buffer address 2     |
   -----------------------------------------------------------------
   | LVDS2HDMI         |   0x80000000     |       0x807e9000       |
   -----------------------------------------------------------------
   | LCD SPEC          |   0x80000000     |       0x803e8000       |
   -----------------------------------------------------------------
   | WAVESHARE_DSI2DPI |   0x80000000     |       0x80258000       |
   -----------------------------------------------------------------

Steps to configure the panels
===============
Default panel is WAVESHARE_DSI2DPI panel, default port is DPU_DI_MIPI, default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is disabled
Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using LVDS2HDMI card(IT6263) only.
When Kconfig is used
----------------
Below setting is for kconfig, you can fix below settings in dpu/figure_display/cm7/prj.conf
For default WAVESHARE_DSI2DPI panel, you do not need to change anything.
For LCD_SPEC panel(1280*800):
CONFIG_LCD_SPEC=y
CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_LVDS=y
For LVDS2HDMI card(1920*1080):
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=y
CONFIG_LVDS2HDMI=y
CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_LVDS=y
When package is used
----------------
Below setting is for mcux_config.h.
For default WAVESHARE_DSI2DPI panel, you do not need to change anything.
Setting DEMO_PANEL to 11 to use LCD_SPEC panel(1280*800), you need to set DPU_EXAMPLE_DI to DPU_DI_LVDS.
Setting DEMO_PANEL to 15 to use LVDS2HDMI card(1920*1080), you need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 1 and set DPU_EXAMPLE_DI to DPU_DI_LVDS.

Running the demo
================
When the example runs successfully, you can see the expected figure shown in screen.
