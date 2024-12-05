Hardware requirements
=====================
- Micro USB cable
- IMX95VERDINEVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- MIPI DSI panel

Board settings
==============
Tips: Need to remove the default verdin DSI to HDMI adapter card first. Then set CSIDSI MINISAS card to DSI/CSI port.
Settings: SW1 = 0 set card output as DSI port. The setting is for MX8_DSI_OLED2 panel and MIPI2HDMI card only.
For MX8_DSI_OLED2 panel(1080*2340), connect MINISAS DSI port to panel.
For MIPI2HDMI card(1920*1080), connect MINISAS DSI port to card.
For CAP_TOUCH_LVDS panel(1280*800), connect LVDS port J24 and Touch port J25 to panel.
Tips: Need to remove the default verdin DSI to HDMI adapter card first. The set Toradex Verdin DSI Display Adapter card to DSI/CSI port.
The setting is for CAP_TOUCH_DSI panel only.
For CAP_TOUCH_DSI  panel(1280*800), connect Toradex Verdin DSI Display Adapter card X2 port to panel.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch S6 to power on the board.
2.  Connect a micro USB cable between the host PC and the J4 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.
6.  Load the figure binary file by using Jlink command "loadbin xxx.bin 0x80000000" and "loadbin xxx.bin 0x809a3f80"(default MX8_DSI_OLED2 panel).

   ------------------------------------------------------------
   | panel name   | buffer address 1 |   buffer address 2     |
   ------------------------------------------------------------
   |MX8_DSI_OLED2 |   0x80000000     |       0x809a3f80       |
   ------------------------------------------------------------
   | MIPI2HDMI    |   0x80000000     |       0x807e9000       |
   ------------------------------------------------------------
   |CAP_TOUCH_LVDS|   0x80000000     |       0x803e8000       |
   ------------------------------------------------------------
   |CAP_TOUCH_DSI |   0x80000000     |       0x803e8000       |
   ------------------------------------------------------------

Steps to configure the panels
===============
Default panel is MIPI DSI panel, default port is DPU_DI_MIPI, default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is disabled
Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) only.

When Kconfig is used
----------------
Below setting is for kconfig, you can fix below settings in dpu/figure_display/cm7/prj.conf
For default MIPI DSI panel, you do not need to change anything.
For MIPI2HDMI card(1920*1080):
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=y
CONFIG_MCUX_PRJSEG_module.board.display_support.MIPI2HDMI=y
For CAP_TOUCH_DSI panel(1280*800):
CONFIG_MCUX_PRJSEG_module.board.display_support.CAP_TOUCH_DSI=y
For CAP_TOUCH_LVDS panel(1280*800):
CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_LVDS=y
CONFIG_MCUX_PRJSEG_module.board.display_support.CAP_TOUCH_LVDS=y

When package is used
----------------
Below setting is for mcux_config.h.
Setting DEMO_PANEL to MIPI2HDMI to use MIPI2HDMI card(1920*1080), you need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 1.
Setting DEMO_PANEL to CAP_TOUCH_DSI to use CAP_TOUCH_DSI panel(1280*800).
Setting DEMO_PANEL to CAP_TOUCH_LVDS to use CAP_TOUCH_LVDS panel(1280*800), you also need to set DPU_EXAMPLE_DI to DPU_DI_LVDS.
For default MIPI DSI panel, you do not need to change anything.

Running the demo
================
When the example runs successfully, you can see the expected figure shown in screen.
