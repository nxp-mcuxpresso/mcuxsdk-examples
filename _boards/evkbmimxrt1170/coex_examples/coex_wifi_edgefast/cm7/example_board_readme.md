## Overview

This document provides step-by-step procedures to build and test coex examples, 
and also instructions for running the included sample applications.

### Hardware requirements

Micro USB cable
evkbmimxrt1170 board
Personal Computer
Embedded Artists 2EL M.2 Module(Rev-A1) - direct M2 connection

### Board settings

Before building the example application define Wi-Fi module in the `_boards/evkbmimxrt1170/coex_examples/coex_wifi_edgefast/cm7/prj.conf`.

If you want to use Embedded Artists 2EL M.2 Module(Rev-A1), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y`


#### Jumper settings for RT1170-EVKB (enables external 5V supply):
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)

#### Murata Solution Board settings
Embedded Artists 2EL module datasheet:  https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVKB and Murata 1XK M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.

**NOTE:**
1. To ensure that the LITTLEFS flash region has been cleaned, all flash sectors need to be erased before downloading example code.
2. After downloaded binary into qspiflash and boot from qspiflash directly, please reset the board by pressing SW7 or power off and on the board to run the application.

## Build and flash

Prerequisites:
- CMake (version >=3.24)
- Ninja (version >=1.12)
- ARM GCC Toolchain (**only support ARM GCC**)
- Python3 (version >=3.6)

> **NOTE**: Make sure that the paths of all these tools are set into the path system variable.

### 2. Building

Modify `examples/coex_examples/coex_wifi_edgefast/app_config.cmake` to generate different coexistence images.

Macors releated to Wi-Fi supplicant,

|   Wi-Fi supplicant   | CONFIG_WPA_SUPPLICANT  |
| -------------------- | ------------------------ |
| embedded supplicant  | 0                        |
| wpa supplicant       | 1(default)               |

### Building coex examples with CMake

> flexspi_nor_debug:
```bash
$ cd <sdk root>
$ west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_edgefast --toolchain armgcc --config flexspi_nor_debug -d build/coex_wifi_edgefast -Dcore_id=cm7
```

> flexspi_nor_release:
```bash
$ cd <sdk root>
$ west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_edgefast --toolchain armgcc --config flexspi_nor_release -d build/coex_wifi_edgefast -Dcore_id=cm7
```

**NOTE:**

> 1. ```-d build/coex_wifi_edgefast``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_edgefast.elf/coex_wifi_edgefast.bin in build/coex_wifi_edgefast folder.
> 3. Only support armgcc to build coex application.

### 4. Flash Binaries

Flash the image with the following command,

```bash
# CMD to write CPU3 coex app image to flash in J-link window:
J-Link> loadbin C:\xxx\coex_wifi_edgefast.bin, 0x30000400
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
SHELL build:         Coex APP
========================================
========================================
     Initialize Firecrest-2EL (IW612) M2 Module
========================================
April 1 2025
Copyright  2020  NXP
========================================

WiFi shell initialization
========================================
BLE shell initialization
@bt> ========================================

@Coex>
```

1. WiFi Test

> NOTE: All wifi commands require adding `wifi ` prefix.

- Get the Wi-Fi driver and firmware version:

```bash
@Coex> wifi wlan-version
WLAN Driver Version   : v1.3.r48.p36
@Coex> WLAN Firmware Version : w9177o-V1, SDIO, FP99, 18.99.3.p23.11, PVE_FIX 1
Command wlan-version
```

- Get MAC Address:

```bash
@Coex> wifi wlan-mac
MAC address
@Coex> STA MAC Address: 50:26:EF:A2:E5:A6
uAP MAC Address: 52:26:EF:A2:E6:A6
Command wlan-mac
```

- Scan the network:

```bash
@Coex> wifi wlan-scan
Scan scheduled...
@Coex> Command wlan-scan
2 networks found:
7C:21:0E:17:B3:8F  "NXP" Infra
        mode: 802.11AC
        channel: 52
        rssi: -66 dBm
        security: WPA2 Enterprise
        WMM: YES
        802.11W: NA
        WPS: NO      
7C:21:0E:17:B3:89  "NXPOPEN" Infra
        mode: 802.11AC
        channel: 52
        rssi: -66 dBm
        security: WPA2
        WMM: YES
        802.11K: YES
        802.11V: YES
        802.11W: NA
        WPS: NO
```

2. Edgefast bluetooth shell Test

- The host must initialized before any bt commands:

> NOTE: Please use the command "help" to view the specific commands supported by the example.

```bash
@Coex> bt init
@Coex> Bluetooth initialized
Settings Loaded

@Coex> bt scan on
Bluetooth active scan enabled
@Coex> [DEVICE]: 57:00:66:0A:D8:AC (random), AD evt type 3, RSSI -92  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 0, RSSI -78  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 4, RSSI -78  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 5C:81:60:9C:36:45 (random), AD evt type 3, RSSI -32  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 59:EE:1D:3A:2D:9C (random), AD evt type 3, RSSI -89  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 0, RSSI -85  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 4, RSSI -86  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 5C:81:60:9C:36:45 (random), AD evt type 3, RSSI -27  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 0A:8B:EF:5C:AC:BC (random), AD evt type 3, RSSI -81  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: C0:95:DA:00:D1:3D (public), AD evt type 0, RSSI -68  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: C0:95:DA:00:D1:3D (public), AD evt type 4, RSSI -68 edgefast_hfp C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 25:E7:96:B7:7F:0E (random), AD evt type 3, RSSI -85  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 48:23:35:4B:F5:84 (public), AD evt type 0, RSSI -92 TVSBT20001915 C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 48:23:35:4B:F5:84 (public), AD evt type 4, RSSI -92  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 0, RSSI -79  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 7B:54:6F:C2:E3:F9 (random), AD evt type 4, RSSI -79  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 74:4D:42:DA:B1:FB (random), AD evt type 3, RSSI -81  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
[DEVICE]: 59:EE:1D:3A:2D:9C (random), AD evt type 3, RSSI -90  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 us), SID: 0xff
@Coex> bt scan off
Scan successfully stopped
@Coex>
```
