Hardware requirements
===================
- Micro USB cable
- TFT Proto 5" CAPACITIVE board HW REV 1.01 by Mikroelektronika. (Named as SSD1963 panel in project. Not necessary if use MIPI panel)
- EVK-MIMXRT595
- Personal Computer
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RM67162 smart MIPI panel (Not necessary if use other panel)

Board settings
============
To use SSD1963 panel:
Connect SSD1963 panel to J43. Touch capability is enabled.

To use MIPI panel:
Connect MIPI panel to J44. Touch capability is enabled.

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

Select the panel
================
RK055MHD091 MIPI panel is used by default, the panel can be switched by using west build option
or modifying mcux_config.h.

When Kconfig is used, use the west build option in this table.
When Package is used, modify `DEMO_PANEL` in mcux_config.h.

| Panel                                                      | west build option          | mcux_config.h DEMO_PANEL |
|------------------------------------------------------------|----------------------------|--------------------------|
| TFT Proto 5" CAPACITIVE (SSD1963)                          | `-DCONFIG_TFT_PROTO_5=y`   | 4                        |
| RK055HDMIPI4M MIPI Rectangular (RK055AHD091)               | `-DCONFIG_RK055AHD091=y`   | 0                        |
| G1120B0MIPI MIPI Circular (RM67162)                        | `-DCONFIG_G1120B0MIPI=y`   | 3                        |
| RK055MHD091A0-CTG MIPI Rectanglular (RK055MHD091, default) | `-DCONFIG_RK055MHD091=y`   | 2                        |


Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
NOTE: As the example is designed for rectangle display, if you use RM67162 circular panel, the content at the
corners will not be seen.
