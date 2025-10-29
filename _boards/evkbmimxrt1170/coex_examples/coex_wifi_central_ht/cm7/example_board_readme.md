## Overview

This document provides step-by-step procedures to build and test coex examples, 
and also instructions for running the included sample applications.

### Hardware requirements

Micro USB cable
evkbmimxrt1170 board
Personal Computer
One of the following modules:
Embedded Artists 2LL M.2 Module (EAR00500) - direct M2 connection
Embedded Artists 2EL M.2 Module (Rev-A1)   - direct M2 connection
Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection
Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection

### Board settings

Before building the example application define Wi-Fi module in the `_boards/evkbmimxrt1170/coex_examples/coex_wifi_central_ht/cm7/prj.conf`.

If you want to use Embedded Artists 2LL M.2 Module(EAR00500), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2ll_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2ll_m2=y`

If you want to use Embedded Artists 2EL M.2 Module (Rev-A1), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y`

If you want to use Embedded Artists 1ZM M.2 Module (EAR00364), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1zm_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.88W8987=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1zm_m2=y`

If you want to use Embedded Artists 1XK M.2 Module (EAR00385), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y`

#### Jumper settings for RT1170-EVKB (enables external 5V supply):
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)

#### Murata Solution Board settings
Embedded Artists 2LL module datasheet:  https://www.embeddedartists.com/wp-content/uploads/2024/12/2LL_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVKB and Murata 2LL M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

**NOTE:**
1. To ensure that the LITTLEFS flash region has been cleaned, all flash sectors need to be erased before downloading example code.
2. After downloaded binary into qspiflash and boot from qspiflash directly, please reset the board by pressing SW4 or power off and on the board to run the application.

## Build and flash

Prerequisites:
- CMake (version >=3.24)
- Ninja (version >=1.12)
- ARM GCC Toolchain (**only support ARM GCC**)
- Python3 (version >=3.6)

> **NOTE**: Make sure that the paths of all these tools are set into the path system variable.

### Building
> flexspi_nor_debug:
```bash
$ cd <sdk root>
$ west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config=flexspi_nor_debug -Dcore_id=cm7 -d coex_wifi_central_ht
```

> flexspi_nor_release:
```bash
$ cd <sdk root>
$ west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config=flexspi_nor_release -Dcore_id=cm7 -d coex_wifi_central_ht
```

**NOTE:**

> 1. ```-d coex_wifi_central_ht``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_central_ht_cm7.elf/coex_wifi_central_ht_cm7.bin in coex_wifi_central_ht folder.
> 3. Only support armgcc to build coex application.

### Flash Binaries

Flash the image with the following command,

```bash
# CMD to write CPU3 coex app image to flash in J-link window:
J-Link> loadbin C:\xxx\coex_wifi_central_ht_cm7.bin, 0x30000400
```

## Run

### Prepare the Demo

1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J86) on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the example.

### Running the example

The log below shows the output of the coex examples in the terminal window:

```bash
========================================
        Coex APP
========================================
========================================
     Initialize Nighthwak-2LL (IW610) M2 Module
========================================
Download BT FW...
download starts(140756)
                       ...........................................................................................................................................
  download success!
                   Download WLAN FW...
STA MAC Address: 78:F5:05:7B:BB:BC
supplicant_main_task: 622 Starting wpa_supplicant thread with debug level: 6

board_type: 0, board_type mapping:
0----CSP
1----QFN
 Coex menu called
========================================
  0  Coex menu print
  w  Wifi Cli
========================================
========================================
========================================
========================================
========================================
BLE Central HT demo start...
                            board_type: 0, board_type mapping:
0----CSP
1----QFN
========================================
app_cb: WLAN: received event 12
========================================
app_cb: WLAN initialized
========================================
WLAN CLIs are initialized
========================================
CLIs Available:
========================================
```

1. WiFi Test

> Enter 'w' to enter Wi-Fi command mode. Enter '0' to return coex menu.

- Get the Wi-Fi driver and firmware version:

```bash
>
>wlan-version
wifi commands: wlan-version
WLAN Driver Version   : v1.3.r48.p37
WLAN Firmware Version : rw610w-V2, IMU, FP99, 18.99.5.p66, PVE_FIX 1
```

- Get MAC Address:

```bash
>wlan-mac
wifi commands: wlan-mac
MAC address
STA MAC Address: 78:F5:05:7B:BB:BC
uAP MAC Address: 7A:F5:05:7B:BC:BC
```

- Scan the network:

```bash
>wlan-scan
wifi commands: wlan-scan
Scan scheduled...

>10 networks found:
AA:A9:30:5F:44:AE  (hidden) Infra
        mode: 802.11AX
        channel: 157
        rssi: -37 dBm
        security: WPA2
        WMM: YES
        802.11V: YES
        802.11W: NA
        WPS: NO
CC:28:AA:2B:DB:D0  (hidden) Infra
        mode: 802.11AX
        channel: 10
        rssi: -44 dBm
        security: WPA2/WPA3 SAE Mixed
        WMM: YES
        802.11K: YES
        802.11V: YES
        802.11W: Capable
        WPS: NO
```

2. BLE Test

> NOTE:
>
> No need enter any BLE commands.
>