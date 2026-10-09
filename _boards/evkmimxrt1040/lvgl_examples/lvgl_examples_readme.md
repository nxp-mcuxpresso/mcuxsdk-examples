Hardware requirements
=====================
- Micro USB cable
- MIMXRT1040-EVK board
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board

Board settings
============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.running the demo.


Steps to configure the panels
===============
The project uses RK043FN66HS-CT6 panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h, change LVGL_PANEL to 0 use RK043FN02H-CT.


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.
XRGB8888 (32-bit), RGB565 (16-bit, default), L8 (8-bit) are supported.

When Kconfig is used
----------------
Add build parameter in `west build` to select different color.
- `-DCONFIG_LV_COLOR_FORMAT_RGB565=y` for RGB565, this is default used if no parameter set.
- `-DCONFIG_LV_COLOR_FORMAT_XRGB8888=y` for XRGB8888
- `-DCONFIG_LV_COLOR_FORMAT_L8=y -DDCONFIG_LV_USE_DRAW_PXP=n` for L8

When package is used
----------------
Modify the mcux_config.h.

To use XRGB8888:
```c
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
#define CONFIG_LV_COLOR_FORMAT_XRGB8888 1
#define CONFIG_LV_COLOR_DEPTH 32
```

To use L8 (8-bit):
```
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
#define CONFIG_LV_COLOR_FORMAT_L8 1
#define CONFIG_LV_COLOR_DEPTH 8
// #define CONFIG_LV_USE_DRAW_PXP 0
```

To use RGB565 (16-bit):
```
#define CONFIG_LV_COLOR_FORMAT_RGB565 1
#define CONFIG_LV_COLOR_DEPTH 16
```

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
