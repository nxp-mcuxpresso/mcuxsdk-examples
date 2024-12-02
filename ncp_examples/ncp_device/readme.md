# ncp_device

## Overview
This is the Wi-Fi CLI example to demonstrate the CLI support usage. The CLI module allows user to add CLIs in application.
Currently only WLAN connection Manager CLIs are available.

Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
For more information about Wi-Fi module connection see:
    readme_modules.txt
    Getting started guide on supported modules configuration:
    https://www.nxp.com/document/guide/getting-started-with-nxp-wi-fi-modules-using-i-mx-rt-platform:GS-WIFI-MODULES-IMXRT-PLATFORM


## Prepare the Demo
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Connect the WiFi module to SD card slot.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Fw download support monolithic
1. To support monolithic, there are three macros that need to be defined according to different scenarios, CONFIG_NCP_WIFI, CONFIG_MONOLITHIC_BLE and CONFIG_MONOLITHIC_BLE_15_4.
2. ncp_device define CONFIG_NCP_WIFI to 1 for wifi.
3. To support wifi + ble, should define CONFIG_MONOLITHIC_BLE to 1 and CONFIG_MONOLITHIC_BLE_15_4 to 0.
4. To support wifi + ot, should define CONFIG_MONOLITHIC_BLE_15_4 to 1 and CONFIG_MONOLITHIC_BLE to 0.
5. To support wifi + ble + ot, should define CONFIG_MONOLITHIC_BLE_15_4 to 1 and CONFIG_MONOLITHIC_BLE to 0.

## Running the demo
1. Load the CPU3 image to 0x08000000.
2. When the demo starts, a welcome message would appear on the terminal:

   NCP device demo
   ========================================
   ========================================
   USB device CDC virtual com demo
   ========================================
   Initialize NCP config littlefs CLIs
   LFS mounted
   Initialize WLAN Driver
   Wi-Fi cau temperature : 28
   MAC Address: C0:95:DA:01:08:3C
   supplicant_main_task: 601 Starting wpa_supplicant thread with debug level: 3
   
   Successfully initialized wpa_supplicant
   iface_cb: iface ml1 ifindex 2 c0:95:da:01:07:3c
   Using interface ml1
   
   Initializing interface 0: ml1
   
   ========================================
   
   MCU wakeup source 0x0...
   PKG_TYPE: QFN
   Set QFN tx power table data
   ========================================
   app_cb: WLAN: received event 12
   ========================================
   app_cb: WLAN initialized
   ========================================
   mDNS are initialized
   ========================================
   ========================================
   app_cb: WLAN: received event 14
   ========================================
   app_cb: WLAN: PS_ENTER
   ========================================
   app_cb: WLAN: received event 14
   ========================================
   app_cb: WLAN: PS_ENTER
## Customization options

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/ncp_examples/ncp_device/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/ncp_examples/ncp_device/example_board_readme.md)
