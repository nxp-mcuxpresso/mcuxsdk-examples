## Overview

This document provides step-by-step procedures to build and test coex examples,
and also instructions for running the included sample applications.

### Hardware requirements

- Micro USB cable
- evkcmimxrt1060 board
- Personal Computer
- One of the following modules:
  - Embedded Artists 2LL M.2 Module (EAR00500) - direct M2 connection

### Board settings

Before building the example application define Wi-Fi module in the `_boards/evkcmimxrt1060/coex_examples/coex_wifi_central_ht/prj.conf`.

If you want to use Embedded Artists 2LL M.2 Module(EAR00500), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2ll_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2ll_m2=y`

#### Jumper settings for RT1060-EVKC (enables external 5V supply):
remove  J40 5-6
connect J40 1-2
connect J45 with external power(controlled by SW6)

#### Murata Solution Board settings
Embedded Artists 2LL module datasheet: https://www.embeddedartists.com/wp-content/uploads/2024/12/2LL_M2_Datasheet.pdf

The hardware should be reworked according to the hardware rework guide for evkcmimxrt1060 and Murata 1XK/1ZM/2EL/2LL M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

**NOTE:**

1. To ensure that the LITTLEFS flash region has been cleaned, all flash sectors need to be erased before downloading example code.
2. After downloaded binary into qspiflash and boot from qspiflash directly, please reset the board by pressing SW7 or power off and on the board to run the application.
3. Nighthwak and Firecrest BT UART Rework (LPUART3)
  - Mount R93, R96
  - Remove R193
  - Connect J109, J76 2-3
4. WiFi and BLE use LPUART.

## Build and flash

Prerequisites:
- CMake (version >=3.24)
- Ninja (version >=1.12)
- ARM GCC Toolchain (**only support ARM GCC**)
- Python3 (version >=3.6)

> **NOTE**: Make sure that the paths of all these tools are set into the path system variable.

### 2. Building
> flexspi_nor_debug:
```bash
$ cd <sdk root>
$ west build -b evkcmimxrt1060 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config flexspi_nor_debug -d coex_wifi_central_ht
```

> flexspi_nor_release:
```bash
$ cd <sdk root>
$ west build -b evkcmimxrt1060 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config flexspi_nor_release -d coex_wifi_central_ht
```

**NOTE:**

> 1. ```-d coex_wifi_central_ht``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_central_ht.elf/coex_wifi_central_ht.bin in coex_wifi_central_ht folder.
> 3. Only support armgcc to build coex application.

### 4. Flash Binaries

Flash the image with the following command,

```bash
# CMD to write CPU3 coex app image to flash in J-link window:
J-Link> loadbin C:\xxx\coex_wifi_central_ht.bin, 0x60000000
```

## Run

### Prepare the Demo

1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the example.

### Running the example

The log below shows the output of the coex examples (based on edgefast-shell) in the terminal window:

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

 Coex menu called
========================================
  0  Coex menu print
  w  Wifi Cli
========================================
BLE Peripheral HT demo start...
                               board_type: 0, board_type mapping:
0----CSP
1----QFN
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
STA MAC Address: C0:95:DA:01:07:36
uAP MAC Address: C2:95:DA:01:08:36
```

- Scan the network:

```bash
>wlan-scan
wifi commands: wlan-scan
Scan scheduled...

>30 networks found:
7C:10:C9:E0:D4:08  "test_ax88U" Infra
        mode: 802.11N
        channel: 1
        rssi: -60 dBm
        security: OPEN
        WMM: YES
        802.11V: YES
        802.11W: NA
04:A1:51:AB:07:1F  "netgear-2g" Infra
        mode: 802.11BG
        channel: 1
        rssi: -56 dBm
        security: WPA
        WMM: YES
        802.11W: NA
```

2. BLE Test

> NOTE:
>
> No need enter any BLE commands.
>
>The demo does not require user interaction. The application will automatically start advertising the Health Thermometer Service and it will accept the first connection request it receives. If the peer subscribes to receive temperature indications, these will be sent every 1 second. The temperature readings are simulated with values between 20 and 25 degrees Celsius.
