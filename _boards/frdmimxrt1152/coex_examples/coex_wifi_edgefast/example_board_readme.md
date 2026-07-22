## Overview

This document provides step-by-step procedures to build and test the coex example
(Wi-Fi + BLE, edgefast_open based) on the FRDM-IMXRT1152 board, and instructions for
running the included sample application.

### Hardware requirements

- Micro USB cable
- FRDM-IMXRT1152 board (on-board u-blox MAYA-W276 / IW612 Wi-Fi + BT module)
- Personal Computer

### Board settings

The Wi-Fi/BT module is on-board (u-blox MAYA-W276, IW612). It is selected by default in
`_boards/frdmimxrt1152/coex_examples/coex_wifi_edgefast/prj.conf`:

> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_ubx_maya_w276=y`

BT HCI runs on LPUART8 (with HW flow control). Wi-Fi runs over USDHC1 (SDIO). The BT and
Wi-Fi reset lines are driven through the on-board PCAL6524 I2C expander.

#### Hardware Rework Guide for FRDM-IMXRT1152

The on-board Bluetooth/Wi-Fi module requires the following board rework before running the
coex application (same rework as the edgefast_open `shell` example on this board):

Bluetooth UART:
- Short:  SJ3, SJ6, SJ7, SJ10
- Remove: R470, R471, SJ38, SJ43, SJ46

Bluetooth Reset:
- Short:  SJ12, SJ23, SJ24

> The BT reset line (BT_RST#, PCAL6524 P0_5) and Wi-Fi reset line (WL_RST#, P0_4) are
> asserted/de-asserted in software via the on-board PCAL6524 I2C expander
> (`BOARD_WIFI_BT_Enable()` in `_boards/frdmimxrt1152/wifi_bt_config.c`). The jumper rework
> above physically routes those reset/UART nets to the module.

**NOTE:**

1. To ensure the LITTLEFS flash region is clean, erase all flash sectors before downloading.
2. After flashing to QSPI flash and booting from it, reset the board by pressing SW1
   (or power cycle) to run the application.
3. Independent NB (BT) firmware download over UART is used, followed by Wi-Fi firmware
   download over SDIO (coex middleware `coex_controller_init()`).

## Build and flash

Prerequisites:
- CMake (version >=3.24)
- Ninja (version >=1.12)
- ARM GCC Toolchain (**only ARM GCC is supported**)
- Python3 (version >=3.6)

### Building

Modify `examples/coex_examples/coex_wifi_edgefast/app_config.cmake` to generate different
coexistence images.

| coexistence images | CONFIG_WIFI | CONFIG_BLE | Simulation Case  |
| ------------------ | ----------- | ---------- | ---------------- |
| WiFi + BLE         | 1           | 1          | Matter over WiFi |

Macros related to Wi-Fi supplicant:

|   Wi-Fi supplicant   | CONFIG_WPA_SUPPLICANT |
| -------------------- | --------------------- |
| embedded supplicant  | 0                     |
| wpa supplicant       | 1 (default)           |

> flexspi_nor_debug:
```bash
$ cd <sdk root>
$ west build -b frdmimxrt1152 examples/coex_examples/coex_wifi_edgefast --toolchain armgcc --config flexspi_nor_debug -d coex_wifi_edgefast
```

> flexspi_nor_release:
```bash
$ cd <sdk root>
$ west build -b frdmimxrt1152 examples/coex_examples/coex_wifi_edgefast --toolchain armgcc --config flexspi_nor_release -d coex_wifi_edgefast
```

**NOTE:**

> 1. ```-d coex_wifi_edgefast``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_edgefast.elf / coex_wifi_edgefast.bin in the coex_wifi_edgefast folder.
> 3. Only ARM GCC is supported to build the coex application.
> 4. CSI and NET_MONITOR are disabled by default due to RAM limitation. To test CSI and
>    NET_MONITOR, enable them and disable enterprise in
>    `middleware/wireless/coex/src/configs/mimxrt1152/wifi/wifi_config.h`:
>    ```
>    #define CONFIG_CSI 1
>    #define CONFIG_NET_MONITOR 1
>    #define CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE 0
>    #define CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE 0
>    ```

### Flash Binaries

```bash
J-Link> loadbin C:\xxx\coex_wifi_edgefast.bin, 0x30000000
```

## Run

### Prepare the Demo

1. Connect a micro USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Reset the board (press SW1) to begin running the example.

### Running the example

After boot, the `@Coex>` shell prompt appears. Both Wi-Fi (with `wifi ` prefix) and BLE
(`bt `) commands are available, e.g.:

```bash
@Coex> wifi wlan-version
@Coex> wifi wlan-scan
@Coex> bt init
@Coex> bt scan on
```

Refer to the evkcmimxrt1060 / evkbmimxrt1170 coex_wifi_edgefast board readme for the full
list of Wi-Fi and BLE command usage examples.
