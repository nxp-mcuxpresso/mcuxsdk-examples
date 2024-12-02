Hardware requirements
=====================
- Micro USB cable
- IMX95LP4XEVK-15 board
- J-Link Debug Probe
- 12V~20V power supply
- MIPI DSI panel
- Personal Computer

Board settings
==============
For MIPI DSI panel(1080*2340), connect MINISAS port J20 to panel.
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

Tips: The APP_DISPLAY_EXTERNAL_CONVERTOR can be set as 1 when you are using MIPI2HDMI card(ADV7535) only.

In this function, the default port is MIPI DSI port. If you want to change panel, please change as below in dpu_example.h
#define DPU_DI_MIPI     1
#define DPU_DI_LVDS     0
#define DPU_EXAMPLE_DI DPU_DI_MIPI
#define MX8_DSI_OLED2   1 (for MIPI DSI panel) or #define MIPI2HDMI   1 (for ADV7535)

Running the demo
================
When the example runs successfully, you can see the rectangles and diamonds
shown in the screen.
