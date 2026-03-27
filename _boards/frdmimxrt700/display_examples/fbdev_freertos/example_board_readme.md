Hardware requirements
===================
- Micro USB cable
- FRDM-IMXRT700
- Personal Computer
- RaspberryPi Panel

Board settings
============
Connect JP1 and JP3.

To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.

Project Configuration
=====================
Refer the section [Display Settings](../../examples_shared_readme.md#display-settings) in file examples_share_readme.md.

Prepare the Demo
===============
The demo use MIPI Raspberry panel by default, to use other panel,
see [Steps to select the panel](../../examples_shared_readme.md#steps-to-select-the-panel).
The supported panels can be found in examples/_boards/frdmimxrt700/project_segments/display_support/Kconfig.prjseg.
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
