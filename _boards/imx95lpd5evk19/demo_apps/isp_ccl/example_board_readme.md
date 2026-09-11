Hardware requirements
=====================
- Micro USB cable (for debug terminal)
- IMX95LPD5-EVK board
- Display connected to LVDS interface
- Cameras connected to CSI
- 12V~20V power supply
- Personal Computer
- (optional) USB-A to USB-A cable for image storing via USB

Board settings
============
use of DDR
- This example is using initialized data in DDR memory. If deployed via SD card or UUU, final binary image needs to be build with spsdk tools. Example templates and scripts are provided in `middleware/neo_isp/isp_sdk/scripts` folder (`build_sd_image.v2.sh`).
  
use of DMA
- This app uses DMA for efficient data transfers. Please make sure that your SM (System manager - m33_image.bin) makes DMA5.2 available for M7 Core.
- Note that you are using precompiled images from imx-boot-tools, they may not allow M7 Core to control DMA 5.2 (Hard Fault!). In such cases, build your own SM or disable MCUX_MISC_middleware.ispsdk.dma_copy_enable option. If option not available in project, replace all utils_EdmaTransfer() calls with memcpy(). Be aware this will SIGNIFICANTLY impact system performance.

For additional information see board user guide and ISP SDK documentation (middleware/neo_isp/isp_sdk/docs/IMX95_ISPSDK_UG_MCUX.md).

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
You should see camera output displayed on the LVDS connected display.
You should see hw statistics every second on the debug terminal.
