Hardware requirements
=====================
- Micro USB cable
- IMX95LPD5EVK-19 board
- J-Link Debug Probe
- 12V~20V power supply
- LCD SPEC panel
- Personal Computer

Board settings
==============
For DUAL LVDS panel(1920*1200), connect MINISAS ports J15 and J16 to panel.
For LCD SPEC panel(1280*800), connect MINISAS port J15 to MINISAS2LVDS card as LVDS0. J16 to MINISAS2LVDS card for LVDS1.
The MINISAS2LVDS card need jumper, please refer to the card datasheet.
For LVDS2HDMI card(1920*1080), connect MINISAS port J16 to panel.
For MIPI DSI panel(1080*2340), connect MINISAS port J14 to panel.
For MIPI2HDMI card(1920*1080), connect MINISAS port J14 to panel.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Steps to configure the panels
===============
Default panel is LCD SPEC panel, default port is DPU_DI_LVDS, default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is disabled
Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) or LVDS2HDMI card(IT6263) only.
When Kconfig is used
----------------
Below setting is for kconfig, you can fix below settings in dpu/wap/cm7/prj.conf
For default LCD_SPEC panel, you do not need to change anything.
For DUAL LVDS panel(1920*1200):
CONFIG_MCUX_PRJSEG_module.board.display_support.LVDS_DUAL_PANEL=y
CONFIG_MCUX_PRJSEG_module.board.display_support.LDB_DUAL_PANEL_ENABLE=y
For LVDS2HDMI card(1920*1080):
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=y
CONFIG_MCUX_PRJSEG_module.board.display_support.LVDS2HDMI=y
For MIPI DSI panel(1080*2340):
CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_MIPI=y
CONFIG_MCUX_PRJSEG_module.board.display_support.MX8_DSI_OLED2=y
For MIPI2HDMI card(1920*1080):
CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_MIPI=y
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=y
CONFIG_MCUX_PRJSEG_module.board.display_support.MIPI2HDMI=y
When package is used
----------------
Below setting is for mcux_config.h.
Setting DEMO_PANEL to LVDS_DUAL_PANEL to DUAL LVDS panel(1920*1200). You also need to set LDB_DUAL_PANEL to 1.
Setting DEMO_PANEL to LVDS2HDMI to use LVDS2HDMI card(1920*1080), you need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 1.
Setting DEMO_PANEL to MX8_DSI_OLED2 to use MIPI DSI panel(1080*2340), you also need to set DPU_EXAMPLE_DI to DPU_DI_MIPI.
Setting DEMO_PANEL to MIPI2HDMI to use MIPI2HDMI card(1920*1080), you need to set DPU_EXAMPLE_DI to DPU_DI_MIPI.
And you also need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 1.
For default LCD_SPEC panel, you do not need to change anything.

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
shown in the screen.
