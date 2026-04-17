Hardware requirements
=====================
- Micro USB cable
- FRDMIMX95 board
- J-Link Debug Probe
- USB power supply
- LVDS2HDMI card
- Personal Computer

Board settings
==============
For LVDS2HDMI card(1920*1080), connect HDMI port J17 to panel.
For LCD_SPEC panel(1280*800), connect LVDS port J14 to panel.
Ensure the J12 jumper connect to 12V, J15 jumper connect to 3.3V

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

Steps to configure the panels
===============
Default panel is LVDS2HDMI card, default port is DPU_DI_LVDS, default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is enabled
Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) or LVDS2HDMI card(IT6263) only.
When Kconfig is used
----------------
Below setting is for kconfig, you can fix below settings in dpu/rop/cm7/prj.conf
For default LVDS2HDMI card, you do not need to change anything.
For LCD_SPEC panel(1280*800):
CONFIG_LCD_SPEC=y
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=n
When package is used
----------------
Below setting is for mcux_config.h.
For default LVDS2HDMI card, you do not need to change anything.
Setting DEMO_PANEL to 11 to use LCD_SPEC panel(1280*800), you need to set APP_DISPLAY_EXTERNAL_CONVERTOR to 0.

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
shown in the screen.
