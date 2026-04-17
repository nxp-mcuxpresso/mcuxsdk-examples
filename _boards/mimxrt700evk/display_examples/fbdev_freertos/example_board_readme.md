Hardware requirements
===================
- Micro USB cable
- MIMXRT700-EVK
- Personal Computer
- TFT Proto 5" CAPACITIVE board HW REV 1.01 by Mikroelektronika. (Named as SSD1963 panel in project. Not necessary if use MIPI panel)
- LCD_PAR_S035 (Not necessary if use MIPI panel)
- RK055AHD091 MIPI panel (Not necessary if use other panel)
- RK055MHD091 MIPI panel (Not necessary if use other panel)
- RaspberryPi Panel (Not necessary if use other panel)
- G1120B0MIPI(RM67162) MIPI Circular Display (Not necessary if use other panel)
- ZC143AC72MIPI(CO5300) MIPI Circular Display (Not necessary if use other panel)

Board settings
============
Connect JP1 and JP3.

To use SSD1963 panel:
Connect SSD1963 panel to J4. Make sure to connect JP7 2&3 to use 3.3v interface.

To use LCD_PAR_S035：
Connect panel to J4 pin 1 to pin 28. Make sure to connect JP7 2&3 to use 3.3v interface, SW1 is 0b011 (8 bit 8080).

To use MIPI panel:
Connect MIPI panel to J52.

To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.
Make sure the R75, R76, R79, R80 are connected.

Project Configuration
=====================
Refer the section [Display Settings](../../examples_shared_readme.md#display-settings) in file examples_share_readme.md.

Prepare the Demo
===============
The demo use MIPI RK055MHD091 panel by default, to use other panel,
see [Steps to select the panel](../../examples_shared_readme.md#steps-to-select-the-panel).
The supported panels can be found in examples/_boards/mimxrt700evk/project_segments/display_support/Kconfig.prjseg.
Or change the macro DEMO_PANEL in `mcux_config.h` of the project,
which number represents which panel can be found in `display_support.h` and `mcux_config.h`

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
its color changes when reached the border.
There is tearing effect when using the SSD1963 panel, because the panel video buffer
could not be fully updated during fresh interval.
