Hardware requirements
===================
- Micro USB cable
- MIMXRT2660-EVK
- Personal Computer
- LCM_RGB_5INCH parallel-RGB panel (default; not necessary if using other panel)
- RK055MHD091A0 MIPI panel (not necessary if using other panel)
- LCD_PAR_S035 panel (not necessary if using other panel)

Board settings
============
Connect the appropriate display panel to the EVK display connector for the
selected panel option.

Project Configuration
=====================
Refer to the section [Display Settings](../../examples_shared_readme.md#display-settings)
in file examples_shared_readme.md.

Prepare the Demo
===============
The demo uses the LCM_RGB_5INCH RGB panel by default. To use another panel,
see [Steps to select the panel](../../examples_shared_readme.md#steps-to-select-the-panel).
The supported panels can be found in
`examples/_boards/mimxrt2660evk/project_segments/display_support/Kconfig.prjseg`.
Or change the macro `DEMO_PANEL` in `mcux_config.h` of the project; the panel
IDs are defined in `display_support.h` and `mcux_config.h`.

1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, you can see a rectangle moving in the screen, and
its color changes when it reaches the border.
