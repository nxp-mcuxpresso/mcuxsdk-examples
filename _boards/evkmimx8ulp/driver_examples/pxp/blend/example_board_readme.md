Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- J-Link Debug Probe
- 5V power supply
- RK055AHD091(rm68200) panel or RK055MHD091(hx8394) panel or monitor(support HDMI input)

Board settings
============
Connect the MIPI panel to MIMX8ULP-EVK board J18.
Connect J1 of board to monitor. (optional: if choose monitor to test the demo, need do this)

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW10 to power on the board.
2.  Connect a micro USB cable between the host PC and the J17 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Build example to generate sdk20-app.bin
5.  Generate flash.bin with imx-mkimage and download it according to Getting Started doc
6.  Open two serials lines provided at J17 USB port.
    (e.g. /dev/ttyUSB0~3, /dev/ttyUSB2 for A Core, /dev/ttyUSB3 for M Core)
7.  Press the reset button on your board and boot to uboot and let uboot not using diplay peripherals
    => setenv video_off yes;saveenv
    then re-power on the board
8.  Let Stop in Uboot.

Steps to configure the panels
===============
RK055MHD091 panel is used by default and the default setting for APP_DISPLAY_EXTERNAL_CONVERTOR is disabled.

When Kconfig is used
----------------
Below setting is for kconfig, you can fix below settings in prj.conf under example folder
For default panel, you do not need to change anything.
For RK055AHD091(rm68200) panel:
CONFIG_MCUX_PRJSEG_module.board.display_support.RK055AHD091=y

For HDMI monitor:
CONFIG_APP_DISPLAY_EXTERNAL_CONVERTOR=y
When package is used
----------------
Below setting is for mcux_config.h.
For RK055AHD091(rm68200) panel:
Setting DEMO_PANEL to 0 to choose RK055AHD091 panel.
For HDMI monitor:
Setting APP_DISPLAY_EXTERNAL_CONVERTOR to 1.

Running the demo
================
When the example runs, the screen shows what described in overview.
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LCDIF pixel clock is: 66000000Hz

PXP Blend example start...
LCDIF pixel clock is: 66000000Hz
MIPI DSI tx_esc_clk frequency: 19800000Hz
MIPI DSI DPHY bit clock: 444000000Hz
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
