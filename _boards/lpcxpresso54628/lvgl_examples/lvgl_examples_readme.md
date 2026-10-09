Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54608 board
- Personal Computer
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board

Board settings
==============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2. Build the project.
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.


Steps to configure the panels
===============
The project uses RK043FN66HS-CT6 panel by default, to use other panels:

If Kconfig is used, select the panel in Kconfig "Panel Selection".

If package is used, modify the mcux_config.h, change LVGL_PANEL to 0 use RK043FN02H-CT.


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.

When Kconfig is used
----------------
Add build parameter in `west build` to select different color.

- RGB565: `-DCONFIG_LV_COLOR_FORMAT_RGB565=y`, or don't add parameter.
- LUT8: `-DCONFIG_LV_COLOR_FORMAT_L8=y`
- LUT8: `-DCONFIG_LV_COLOR_FORMAT_I1=y`

When package is used
----------------
Modify the mcux_config.h:

To use 8-bit color depth, change like
```
#define CONFIG_LV_COLOR_FORMAT_L8 1
#define CONFIG_LV_COLOR_DEPTH 8
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
```

To use 16-bit color depth, change like
```
#define CONFIG_LV_COLOR_FORMAT_RGB565 1
#define CONFIG_LV_COLOR_DEPTH 16
```

To use 1-bit color depth, change like
```
#define CONFIG_LV_COLOR_FORMAT_I1 1
#define CONFIG_LV_COLOR_DEPTH 1
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
```

Running the demo
================
If this example runs correctly, the sample GUI is displayed.
