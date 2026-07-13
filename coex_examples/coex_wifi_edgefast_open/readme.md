# coex_wifi_edgefast_open

## Overview
This example demonstrates Wi-Fi and Bluetooth coexistence on the same platform.
It combines the Wi-Fi CLI functionality (from wifi_cli) with the Edgefast Open
Bluetooth shell (from edgefast_open/shell), allowing users to operate both Wi-Fi
and BT simultaneously through a unified shell interface.

The application downloads the combo (Wi-Fi + BT) firmware over SDIO, initializes
the Wi-Fi stack, and then starts the Edgefast Open BT shell. Wi-Fi commands are
accessible via the `wifi` prefix on the BT shell prompt.

## Supported Boards
- EVK-BMIMXRT1170 (CM7)
- FRDM-IMXRT700 (CM33 core0)
- MIMXRT1170-EVKB

## Supported Modules
- IW612 (Murata 2EL M2) — default
- IW416 (Murata 1XK M2) — evkbmimxrt1170 only

To switch modules, edit the board-level `prj.conf` and change:
```
# For IW612 (default):
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y

# For IW416:
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y
```

## Features
- Full Wi-Fi CLI commands via `wifi` prefix (scan, connect, ping, iperf, etc.)
- Full Bluetooth shell commands (advertise, scan, connect, GATT, L2CAP, ISO, etc.)
- Concurrent Wi-Fi + BT operation
- Combo firmware download (Wi-Fi + BT shared over SDIO)

## Build
```
# evkbmimxrt1170
west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_edgefast_open \
  -p always -d build_coex_edgefast_open --config flexspi_nor_release \
  -- -Dcore_id=cm7 -DCONFIG_TOOLCHAIN=armgcc

# frdmimxrt700
west build -b frdmimxrt700 examples/coex_examples/coex_wifi_edgefast_open \
  -p always -d build_coex_edgefast_open_700 --config flash_release \
  -- -Dcore_id=cm33_core0 -DCONFIG_TOOLCHAIN=armgcc
```

## Running the demo
1. Connect the board to the host PC via USB debug port.
2. Open a serial terminal with 115200 baud rate.
3. Reset the board.
4. Wait for the Wi-Fi initialization to complete and the BT shell prompt (uart:~$) to appear.

## Usage

### Wi-Fi Commands
All Wi-Fi commands are accessed via the `wifi` prefix:
```
uart:~$ wifi help              - List all available Wi-Fi commands
uart:~$ wifi wlan-scan         - Scan for available networks
uart:~$ wifi wlan-version      - Show WLAN driver and firmware version
uart:~$ wifi wlan-mac          - Show MAC address
uart:~$ wifi wlan-add <profile> ssid <ssid>
                               - Add an open network profile
uart:~$ wifi wlan-add <profile> ssid <ssid> wpa2 <passphrase>
                               - Add a WPA2 network profile
uart:~$ wifi wlan-connect <profile_name>
                               - Connect to a saved network
uart:~$ wifi wlan-disconnect   - Disconnect from network
uart:~$ wifi wlan-stat         - Show connection status
uart:~$ wifi wlan-address      - Show IP address
uart:~$ wifi ping <ip>         - Ping an IP address
uart:~$ wifi iperf -s          - Start iperf server
uart:~$ wifi iperf -c <ip> -u -B <local_ip>
                               - Run iperf UDP client
```

### Bluetooth Commands
BT commands are used directly (no prefix needed):
```
uart:~$ bt init                - Initialize Bluetooth
uart:~$ bt scan on             - Start BLE scanning
uart:~$ bt scan off            - Stop BLE scanning
uart:~$ bt advertise on        - Start BLE advertising
uart:~$ bt advertise off       - Stop BLE advertising
uart:~$ bt connect <addr> <type> - Connect to a BLE device
uart:~$ bt disconnect          - Disconnect
```

Type `help` at the prompt to see all available commands including BT subcommands
(gatt, l2cap, iso).

## Expected Boot Output
```
========================================
coex_wifi_edgefast_open demo
========================================
========================================
     Initialize Firecrest-2EL (IW612) M2 Module
========================================
[FW Download] Start to download firmware from 0x...: ...
[FW Download] WLAN FW is active: ...
STA MAC Address: XX:XX:XX:XX:XX:XX
WLAN started successfully
app_cb: WLAN initialized
========================================
WLAN CLIs are initialized
========================================
ENHANCED WLAN CLIs are initialized
========================================
PING CLI is initialized
========================================
IPERF CLI is initialized
========================================
Starting BT shell...
========================================
Edgefast Bluetooth PAL shell demo start...
uart:~$
Wi-Fi commands registered. Use: wifi <command>
Type 'wifi help' for available Wi-Fi commands
```

## Architecture
- **task_main**: Initializes WLAN driver (downloads combo firmware), waits for
  WLAN_REASON_INITIALIZED callback, then starts the BT shell (shell_task).
- **wifi_cmd_register_task**: Waits for the BT shell to initialize, then registers
  the wifi command on it via SHELL_RegisterCommand.
- **cmd_wifi**: Uses lookup_command() to find and execute Wi-Fi CLI commands
  directly in the shell task context. Requires -DCOEX_APP_SUPPORT=1.

## Known Limitations
- Wi-Fi and BT share the same combo firmware. BT firmware download is bypassed
  (CONFIG_BT_BYPASS_DOWNLOAD=y) since it is included in the combo image.
- The wifi command output may appear asynchronously (e.g., scan results print
  after the prompt returns).
- IW416 support is available on evkbmimxrt1170 only.
