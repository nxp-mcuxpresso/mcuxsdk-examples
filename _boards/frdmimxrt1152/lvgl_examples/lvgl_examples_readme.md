Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel or RaspberryPi panel

Board settings
============
Connect the panel to J62. For RaspberryPi panel, connect the panel to J58,
then connect the panel's 5V pin to J15-6, GND pin to J15-8.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Steps to configure the panels
===============
The project uses RK055MHD091A0-CTG panel by default, to use other panels:

| Panel                                                      | west build option          | mcux_config.h DEMO_PANEL |
|------------------------------------------------------------|----------------------------|--------------------------|
| RK055HDMIPI4M MIPI Rectangular (RK055AHD091)               | `-DCONFIG_RK055AHD091=y`   | 0                        |
| MIPI RK055IQH091 panel                                     | `-DCONFIG_RK055IQH091=y`   | 1                        |
| RK055MHD091A0-CTG MIPI Rectanglular (RK055MHD091, default) | `-DCONFIG_RK055MHD091=y`   | 2                        |
| RaspberryPi                                                | `-DCONFIG_RASPI_7INCH=y`   | 5                        |


Steps to configure the pixel format
===============
The project uses RGB565 pixel format by default.
RGB565, XRGB888(32-bit), LUT8 formats are support.

When Kconfig is used
----------------
Add build parameter in `west build` to select different color.

- XRGB8888: `-DCONFIG_LV_COLOR_FORMAT_XRGB8888=y -DCONFIG_DEMO_USE_XRGB8888=y`
- RGB565: `-DCONFIG_LV_COLOR_FORMAT_RGB565=y`, or don't add parameter.
- LUT8: `-DCONFIG_LV_COLOR_FORMAT_L8=y -DCONFIG_DEMO_USE_LUT8=y -DCONFIG_LV_USE_DRAW_VG_LITE=n -DDCONFIG_LV_USE_DRAW_PXP=n`

When package is used
----------------
Modify the mcux_config.h.

To use XRGB8888,
```c
#define DEMO_USE_XRGB8888 1
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
#define CONFIG_LV_COLOR_FORMAT_XRGB8888 1
#define CONFIG_LV_COLOR_DEPTH 32
```

To use 8-bit color depth, change like
```
#define DEMO_USE_XRGB8888 0
#define DEMO_USE_LUT8 1
#define CONFIG_LV_COLOR_FORMAT_L8 1
#define CONFIG_LV_COLOR_DEPTH 8
// #define CONFIG_LV_COLOR_FORMAT_RGB565 0 NOTE: Must remove or comment this line
// #define CONFIG_LV_USE_DRAW_VG_LITE 0
// #define CONFIG_LV_USE_VECTOR_GRAPHIC 0
// #define CONFIG_LV_USE_DRAW_PXP 0
```

To use 16-bit color depth, change like
```
#define CONFIG_LV_COLOR_FORMAT_RGB565 1
#define CONFIG_LV_COLOR_DEPTH 16
```

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
