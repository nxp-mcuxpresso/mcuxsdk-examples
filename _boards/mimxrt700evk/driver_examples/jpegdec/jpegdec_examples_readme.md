Hardware requirements
=====================
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
Connect the MIPI panel to MIMXRT700-EVK board J105.
For single_stream_repeat and context_switch examples, make sure JP65 1-2 are connected, and insert the
SD card which has the pictures unziped in root folder in slot J47.

Prepare the Demo
===============
The demo use MIPI RK055MHD091 panel by default, to use other panel,
set the coresponding panel DEMO_PANEL_xxx(without '_SUPPORT') to y in prj.conf under board example folder before project generation.
The supported panels can be found in examples\_boards\<board>\project_segments\display_support\Kconfig.prjseg.
Or change the macro DEMO_PANEL in mcux_config.h of the project,
which number represents which panel can be found in display_support.h and mcux_config.h

1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J54) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, this project uses RK055MHD091 panel by default, to use use other panel,
	change the macro DEMO_PANEL in mcux_config.h. Which number represents which panel
	can be found in display_support.h and mcux_config.h.
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs, the screen shows what described in overview.
