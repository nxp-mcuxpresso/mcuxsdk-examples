# coex_wifi_spp

## Overview
The `coex_wifi_spp` application demonstrates Wi-Fi + Bluetooth Classic
coexistence on a single NXP wireless module (IW612 or IW416). It combines the
edgefast_open Bluetooth **SPP (Serial Port Profile)** example with the Wi-Fi CLI,
running both radios concurrently through the shared `middleware/wireless/coex`
glue with **independent NB (BT) firmware download** over UART followed by WLAN
init over SDIO.

A single serial shell (`@Coex>`) exposes three command groups:
- `bt`   - BR/EDR connection control (discover / connect / disconnect / delete)
- `spp`  - SPP RFCOMM control (register / connect / uuidconnect / disconnect / send)
- `wifi` - Wi-Fi CLI dispatch (`wifi wlan-scan`, `wifi wlan-connect`, `wifi ping`,
           `wifi iperf`, ...). Wi-Fi commands only work after the WLAN interface
           has come up (registered by the coex middleware on initialization).

## Prepare the Demo
1. Connect a USB cable between the PC host and the debug USB port on the board.
2. Open a serial terminal with 115200 baud, 8N1, no flow control.
3. Build and download the program to the target board.
4. Reset the board to start running the demo.

## Running the Demo
After boot, the coex prompt appears:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
========================================
        Coex SPP APP
========================================
...
host init done
Bluetooth SPP demo start...
Bluetooth initialized
@Coex>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Type `help` to list all commands. Use the group prefix for each command, e.g.
`bt discover`, `spp register 3`, `wifi wlan-scan`.

## Available Commands

### Bluetooth (bt) Commands
- `bt discover`         - Start discovery to find nearby Bluetooth devices
- `bt connect <index>`  - Connect to a discovered device (index from 1)
- `bt disconnect`       - Disconnect current Bluetooth connection
- `bt conns`            - Print all active Bluetooth connections
- `bt switch <index>`   - Switch between multiple Bluetooth connections
- `bt delete`           - Delete all bonding information (disconnect first!)

### SPP Commands
- `spp register <channel>`   - Register SPP server on RFCOMM channel (1-30)
- `spp connect <channel>`    - Connect to remote SPP server by RFCOMM channel
- `spp uuidconnect [uuid]`   - Connect to remote SPP service by UUID (SDP discovery)
- `spp disconnect`           - Disconnect current SPP connection
- `spp send <1|2|3|4>`       - Send test data over SPP connection

### Wi-Fi (wifi) Commands
- `wifi help`                - List the available WLAN CLI commands
- `wifi wlan-scan`           - Scan for Wi-Fi networks
- `wifi wlan-add ...` / `wifi wlan-connect ...` - Add / connect to a network
- `wifi ping <ip>`           - Ping test
- `wifi iperf ...`           - iperf throughput test

## Coexistence Usage
1. Establish an SPP session (see the SPP examples below).
2. While the SPP link is up, run `wifi wlan-scan`, connect to an AP with
   `wifi wlan-add`/`wifi wlan-connect`, then `wifi ping` / `wifi iperf` to verify
   simultaneous Wi-Fi + BT operation.

## SPP Usage Examples

### Example 1: SPP Server
1. Enter `spp register 3` to register an SPP server on RFCOMM channel 3.
2. Wait for a remote device (the SPP client board) to connect.
3. After the connection is established, enter `spp send 1` to send test data.
4. Enter `spp disconnect` to disconnect when done.

### Example 2: SPP Client (Connect by Channel)
1. Enter `bt discover` to find nearby Bluetooth devices (the server shows as `edgefast_spp`).
2. Wait for discovery to complete and note the device index.
3. Enter `bt connect <index>` to establish the BR/EDR link.
4. Enter `spp connect 3` to create the SPP connection on RFCOMM channel 3.
5. After the SPP connection is established, enter `spp send 2` to send test data.
6. Enter `spp disconnect`, then `bt disconnect` to tear down.

### Example 3: SPP Client (Connect by UUID)
1. Enter `bt discover`, then `bt connect <index>` to connect to the server.
2. Enter `spp uuidconnect` to connect using the standard SPP UUID (auto SDP discovery),
   or specify a custom UUID: `spp uuidconnect 00001101-0000-1000-8000-00805F9B34FB`.
3. After the connection is established, enter `spp send 3` to send test data.
4. Enter `spp disconnect` when done.

## Notes
- Maximum supported connections: 2 (configurable via `CONFIG_BT_MAX_CONN`).
- RFCOMM channel range: 1-30 (channel 0 is reserved).
- The device is automatically set to connectable and discoverable on startup.
- Bonding information is stored; use `bt delete` to clear it (disconnect first).
- SPP data received from the peer is displayed in hexdump format.

## Wi-Fi/BT Module Selection
Default is IW612 (Murata 2EL M2). To use IW416 (Murata 1XK M2), edit the board
`prj.conf` in `_boards/evkbmimxrt1170/coex_examples/coex_wifi_spp/cm7/prj.conf`:

~~~
# For IW612 (default):
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y

# For IW416:
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y
~~~

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/coex_examples/coex_wifi_spp/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/coex_examples/coex_wifi_spp/example_board_readme.md)
