## Overview

This document provides the board-specific hardware setup for building and
running the `coex_wifi_spp` (Wi-Fi + Bluetooth SPP coexistence) example on the
MIMXRT1170-EVKB.

### Hardware requirements

- Micro USB cable
- evkbmimxrt1170 board
- Personal Computer
- One of the following M.2 modules (direct M2 connection):
  - Embedded Artists 1XK M.2 Module (EAR00385) - IW416
  - Embedded Artists 2EL M.2 Module (Rev-A1)    - IW612

### Board settings

Before building the example, select the Wi-Fi/BT module in
`_boards/evkbmimxrt1170/coex_examples/coex_wifi_spp/cm7/prj.conf`:

If you want to use the Embedded Artists 2EL M.2 Module (Rev-A1) - IW612 (default):
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y`

If you want to use the Embedded Artists 1XK M.2 Module (EAR00385) - IW416:
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y`

#### Jumper settings for RT1170-EVKB (enables external 5V supply):
```
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)
```

#### Murata Solution Board settings
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the Hardware Rework Guide for
MIMXRT1170-EVKB and the Murata 1XK/2EL M.2 Adapter in the document
"Hardware Rework Guide for EdgeFast Open Bluetooth Host stack". BT HCI is carried
over the M2 UART (LPUART2); Wi-Fi over SDIO (USDHC).

**NOTE:**
1. To ensure that the LITTLEFS flash region has been cleaned, all flash sectors
   need to be erased before downloading example code.
2. After downloading the binary into QSPI flash and booting from QSPI flash
   directly, please reset the board by pressing SW4 (or power off and on the
   board) to run the application.

### Prepare the Demo
1. Connect a micro USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal with 115200 baud, 8 data bits, no parity, 1 stop bit, no flow control.
3. Build the project with the `flexspi_nor_release` configuration and the armgcc
   toolchain for the `cm7` core.
4. Download the program to the target board.
5. Reset the board to begin running the demo.

See the top-level `readme.md` of `coex_wifi_spp` for the full command list and
Wi-Fi + SPP coexistence usage.
