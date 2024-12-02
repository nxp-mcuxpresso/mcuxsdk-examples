Hardware requirements
=====================
- Micro USB cable
- IMX95LPD5EVK-19 board
- J-Link Debug Probe
- 12V~20V power supply
- DUAL LVDS panel
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

Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) or LVDS2HDMI card(IT6263) only.

In this function, the default port is LVDS port. If you want to use DPU_DI_MIPI, please change as below in dpu_example.h
#define DPU_DI_MIPI     1
#define DPU_DI_LVDS     0
#define LVDS_DUAL_PANEL 0
#define DPU_EXAMPLE_DI  DPU_DI_MIPI
#define MX8_DSI_OLED2   1 (for MIPI DSI panel) or #define MIPI2HDMI   1 (for ADV7535)

In this function, the default panel is DUAL LVDS panel. If you want to use LCD SPEC panel, please change as below in dpu_example.h
#define LVDS_DUAL_PANEL 0
#define LCD_SPEC        1
#define DPU_LVDS_PANEL LCD_SPEC
If you want to use LVDS2HDMI card(IT6263), please change as below in dpu_example.h
#define LVDS_DUAL_PANEL 0
#define LVDS2HDMI       1
#define DPU_LVDS_PANEL  LVDS2HDMI

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
shown in the screen.
