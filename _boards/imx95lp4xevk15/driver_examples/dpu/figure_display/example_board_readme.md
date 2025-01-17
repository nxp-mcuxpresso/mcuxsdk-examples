Hardware requirements
=====================
- Micro USB cable
- IMX95LP4XEVK-15 board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- MIPI DSI panel

Board settings
==============
For MX8_DSI_OLED2 MIPI panel(1080*2340), connect MINISAS port J20 to panel.
For MIPI2HDMI card(1920*1080), connect MINISAS port J20 to card.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW5 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.
6.  Load the figure binary file by using Jlink command "loadbin xxx.bin 0x80000000" and "loadbin xxx.bin 0x809a3f80"(default MIPI DSI panel).

   ------------------------------------------------------------
   | panel name   | buffer address 1 |   buffer address 2     |
   ------------------------------------------------------------
   |MX8_DSI_OLED2 |   0x80000000     |       0x809a3f80       |
   ------------------------------------------------------------
   | MIPI2HDMI    |   0x80000000     |       0x807e9000       |
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

When package is used
----------------
Below setting is for mcux_config.h.
Setting DEMO_PANEL to MIPI2HDMI to use MIPI2HDMI card(1920*1080), you need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 1.
For default MIPI DSI panel, you do not need to change anything.

Running the demo
================
When the example runs successfully, you can see the expected figure shown in screen.
