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

Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) only.

In this function, the default port is MIPI DSI port. If you want to change panel, please change as below in dpu_example.h
#define DPU_DI_MIPI     1
#define DPU_DI_LVDS     0
#define DPU_EXAMPLE_DI DPU_DI_MIPI
#define MX8_DSI_OLED2   1 (for RM692c9 panel) or #define MIPI2HDMI   1 (for ADV7535) or  #define CAP_TOUCH_DSI   1(for SN65DSI83 panel)
In this function, the default port is MIPI DSI port. If you want to use LVDS panel, please change as below in dpu_example.h
#define DPU_DI_MIPI     0
#define DPU_DI_LVDS     1
#define CAP_TOUCH_LVDS  1
#define MX8_DSI_OLED2   0
#define DPU_EXAMPLE_DI DPU_DI_LVDS
#define DPU_LVDS_PANEL CAP_TOUCH_LVDS

Running the demo
================
When the example runs successfully, you can see the expected figure shown in screen.
