Hardware requirements
=====================
- Micro USB cable
- IMX952LPD5EVK-19 board
- J-Link Debug Probe
- 12V~20V power supply
- MX9_LVDS_LD panel
- Personal Computer

Board settings
==============
For MX9_LVDS_LD panel(1920*720), connect MINISAS ports J15 and J16 to panel.

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
Default panel is MX9_LVDS_LD panel, default port is DPU_DI_LVDS, default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is disabled
Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) or LVDS2HDMI card(IT6263) only.
When Kconfig is used
----------------
Below setting is for kconfig, the settings in dpu/localdimming/cm7/prj.conf
For default MX9_LVDS_LD panel, you do not need to change anything.
When package is used
----------------
Below setting is for mcux_config.h.
For default MX9_LVDS_LD panel, Setting DEMO_PANEL to 18. You also need to set LDB_DUAL_PANEL to 1.

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
shown in the screen.
