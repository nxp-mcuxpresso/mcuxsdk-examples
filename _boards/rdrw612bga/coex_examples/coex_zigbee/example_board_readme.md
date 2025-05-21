## Hardware requirements

- Micro USB cable
- RD-RW612-BGA board
- Personal Computer

## Prepare the Demo

1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the example.

## Building Coex Zigbee examples with CMake

Prerequisites:
- CMake (version >=3.24)
- Ninja (version >=1.12)
- ARM GCC Toolchain (**only support ARM GCC**)
- Python3 (version >=3.6)

### Python requirements

Python is used to run the ZPSConfig and PDUMConfig tools, and requires the extra modules. To avoid conflicts with your global Python install, we recommend using a virtual environment. You can either use the one used with your MCUXPresso SDK install, or create a specific one:
```bash
python3 -m venv .venv
```

Then, make sure to activate your environment:
```bash
source .venv/bin/activate
```
Once the virtual environment is activated, you can install the required modules with `pip`:
```bash
pip install -r requirements.txt
```

### Windows long paths limitation
CMake can generate long paths name, and depending on where your MCUX SDK is located, the build might not work correctly on Windows. For this reason, it is recommended to enable long paths support, Microsoft documented the procedure [here](https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=registry). We also recommend to place your MCUXPresso SDK in the root of your disk, like `C:\`, to reduce the paths length as much as possible.

Currently, a Linux environment is preferred over Windows due to these limitations. WSL, Virtual Machine or native Linux environment can be used.

## Build and Flash

Please setup the SDK. Once the SDK is setup, the coex_zigbee examples will be located at `<path to sdk>/examples/coex_examples/coex_zigbee` and you will be able to build the CMake based examples. 

### Configuration

Modify `examples/coex_examples/coex_zigbee/app_config.cmake` to generate different coexistence images.

Macros releated to Wi-Fi/BLE component:

| coexistence images | CONFIG_ZIGBEE | CONFIG_BLE | CONFIG_WIFI |
| - | - | - | - |
| Zigbee + Wi-Fi       | 1 | 0 | 1 |
| Zigbee + BLE         | 1 | 1 | 0 |
| Zigbee + BLE + Wi-Fi | 1 | 1 | 1 |

Modify these options according to your needs:

|   Wi-Fi supplicant   | CONFIG_WPA_SUPP_MBEDTLS  |
| -------------------- | ------------------------ |
| embedded supplicant  | 0                        |
| wpa-supplicant       | 1(default)               |


### Building

To build a coex_zigee example, we use `west build`, like the following:
```bash
west build -b <board> <sdk root>/examples/coex_examples/coex_zigbee/<zigbee role>/freertos --toolchain armgcc
```

Build the coex_zigbee application of rdrw612bga board:
```bash
# For coex_zigbee_coordinator example,
west build -b rdrw612bga <sdk root>/examples/coex_examples/coex_zigbee/coordinator/freertos --toolchain armgcc

# For coex_zigbee_router example,
west build -b rdrw612bga <sdk root>/examples/coex_examples/coex_zigbee/router/freertos --toolchain armgcc
```

> **NOTE**: 
> For `coex_zigbee_router` examples, still need to build `mcuboot_opensource` image.
```bash
# For mcuboot_opensource example,
west build -b rdrw612bga examples/ota_examples/mcuboot_opensource
```

### Current examples supported

You'll find below the list of coex_zigbee examples supported with CMake.

All these examples are located with the other SDK examples, in `<sdk_root>/examples`.

| Name | Source folder | Description | Supported boards |
| - | - | - | - |
| `coex_zigbee_coordinator` | `<sdk_root>/examples/coex_examples/coex_zigbee/coordinator/freertos` | See [README](https://bitbucket.sw.nxp.com/projects/CONNINT/repos/zigbee_public/browse/examples/zigbee_coordinator/README.md) | `frdmrw612` `rdrw612bga` |
| `coex_zigbee_router` | `<sdk_root>/examples/coex_examples/coex_zigbee/router/freertos` | See [README](https://bitbucket.sw.nxp.com/projects/CONNINT/repos/zigbee_public/browse/examples/zigbee_router/README.md) | `frdmrw612` `rdrw612bga` |


### Flash Binaries

Flash the image with the following command,
```bash
# CMD to write CPU3 coex app image to flash in J-link window:

# Flash coex_zigbee_coordinator,
J-Link> loadfile coex_zigbee_coordinator.elf

# Flash coex_zigbee_router,
J-Link> loadfile mcuboot_opensource.elf
J-Link> loadbin coex_zigbee_router_v1.signed.confirmed.bin 0x18020000
```

## Running the example

The log below shows the output of the coex examples (based on edgefast-shell) in the terminal window:

```bash
        Coex APP

SHELL build: ========================================
========================================
     Initialize RW612 Module
========================================
May 14 2024
Copyright  2020  NXP
========================================

WiFi shell initialization
========================================
BLE shell initialization
@bt> ========================================

@Coex>
```

1. WiFi Test

> NOTE: All wifi commands require adding `wifi.` prefix.

- Get the Wi-Fi driver and firmware version:

```bash
@Coex> wifi.wlan-version
WLAN Driver Version   : v1.3.r48.p10
@Coex> WLAN Firmware Version : rw610w-V2, IMU, FP99, 18.99.6.p6, PVE_FIX 1
Command wlan-version
```

- Get MAC Address:

```bash
@Coex> wifi.wlan-mac
MAC address
@Coex> STA MAC Address: 00:50:43:02:98:23
uAP MAC Address: 00:50:43:02:99:23
Command wlan-mac
```

- Scan the network:

```bash
@Coex> wifi.wlan-scan
Scan scheduled...
@Coex> Command wlan-scan
2 networks found:
1C:6A:7A:87:FF:BF  "NXP" Infra
        mode: 802.11AC
        channel: 161
        rssi: -74 dBm
        security: WPA2 Enterprise
        WMM: YES
        802.11K: YES
        802.11V: YES
        802.11W: NA
        WPS: NO
1C:6A:7A:87:FF:BE  "NXPOPEN" Infra
        mode: 802.11AC
        channel: 161
        rssi: -75 dBm
        security: WPA2
        WMM: YES
        802.11K: YES
        802.11V: YES
        802.11W: NA
        WPS: NO
```

2. BLE Test

> NOTE: Please use the command "help" to view the specific commands supported by the example.

- BLE scan devices (the BLE host must initialized before):

```bash
@Coex> bt.init
@Coex> Bluetooth initialized
Settings Loaded

@Coex> bt.scan on
Bluetooth active scan enabled
@Coex> [DEVICE]: 44:6D:F5:85:DC:5F (random), AD evt type 0, RSSI -64  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 44:6D:F5:85:DC:5F (random), AD evt type 4, RSSI -63  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 6D:B3:D3:8E:ED:A2 (random), AD evt type 0, RSSI -77  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 6D:B3:D3:8E:ED:A2 (random), AD evt type 4, RSSI -76  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 3F:FB:95:F7:F9:14 (random), AD evt type 3, RSSI -75  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 49:A3:4E:86:63:0C (random), AD evt type 0, RSSI -76  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 49:A3:4E:86:63:0C (random), AD evt type 4, RSSI -75  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 5C:28:50:F9:DD:57 (random), AD evt type 0, RSSI -82  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 4A:7D:B4:12:7B:7A (random), AD evt type 0, RSSI -82  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 4A:7D:B4:12:7B:7A (random), AD evt type 4, RSSI -82  C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 5A:54:C8:99:13:4A (random), AD evt type 0, RSSI -76  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 3B:95:00:4D:F3:EB (random), AD evt type 3, RSSI -82  C:0 S:0 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 47:9D:D0:CB:5F:0D (random), AD evt type 0, RSSI -86  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
@Coex> bt.scan off
Scan successfully stopped
@Coex>
```

- BLE advertise (the BLE host must initialized before):

```bash
@Coex> bt.init
@Coex> Bluetooth initialized
@Coex> bt.advertise on
Advertising started
@Coex> bt.advertise off
Advertising stopped
```

- BLE connect (the BLE host must initialized before):

```bash
@Coex> bt.init
@Coex> Bluetooth initialized
@Coex> bt.connect C0:95:63:23:55:87 random
Connection pending
Connected: 7D:FD:FD:4D:FD:90 (random)
```

- BLE pairing and bonding:

```bash
GATT peripheral role side,
1. Initialize the Host, press "bt.init",
2. Advertising, press "bt.advertise on",
3. After the connection is established, perform the pairing sequence,
   it could be started from peripheral side by pressing "bt.security <level>", such as "bt.security 2".
4. If the bondable is unsupported by peripheral role, press "bt.bondable off". Then start step 3.

GATT central role side,
1. Initialize the Host, press "bt.init",
2. Scaning advertising packets, press "bt.scan on",
3. A few seconds later, stop the scanning, press "bt.scan off"
4. Select the target board and create a new connection. If the taregt is not listed, repeat steps 2 and 3.
   Then press "bt.connect <address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"
5. After the connection is established, perform the pairing sequence,
   it could be started from central side by pressing "bt.security <level>", such as "bt.security 2".
6. If the bondable is unsupported by central role, press "bt.bondable off". Then start step 5.
```

- BLE 1M/2M/Coded PHY update:

```bash
GATT peripheral role side,
1. Initialize the Host, press "bt.init",
2. Advertising, press "bt.advertise on",
3. After the connection is established.
4. Send phy update command, press "bt.phy-update <tx_phy> [rx_phy] [s2] [s8]", tx_phy/rx_phy could be 1(1M) or 2(2M) or 4(Coded).
   such as "bt.phy-update 2 2".
5. The message "LE PHY updated: TX PHY LE 2M, RX PHY LE 2M" would be printed if the phy is updated. note, if peer do not support phy update, then this message will not be printed.

GATT central role side,
1. Initialize the Host, press "bt.init",
2. start scan, press "bt.scan on", Bluetooth device around your current bluetooth will be list, for example,
[DEVICE]: 72:78:C1:B5:0F:DA (random), AD evt type 4, RSSI -32 BLE Peripheral C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: C4:0D:02:55:5E:AD (random), AD evt type 0, RSSI -83  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
[DEVICE]: 66:8F:26:27:1F:52 (random), AD evt type 0, RSSI -82  C:1 S:1 D:0 SR:0 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
3. stop scan, press "bt.scan off",
4. connect target device, press "bt.connect <address: XX:XX:XX:XX:XX:XX> <type: (public|random)>", such as bt.connect 72:78:C1:B5:0F:DA random
5. Send phy update command, press "bt.phy-update <tx_phy> [rx_phy] [s2] [s8]", tx_phy/rx_phy could be 1(1M) or 2(2M) or 4(Coded).
   such as "bt.phy-update 2 2".
6. The message "LE PHY updated: TX PHY LE 2M, RX PHY LE 2M" would be printed if the phy is updated. note, if peer do not support phy update, then this message will not be printed.
```

- BLE Data Packet Length Extension update:

```bash
GATT peripheral role side,
1. Initialize the Host, press "bt.init".
2. Advertising, press "bt.advertise on".
3. After the connection is established.
4. Check current LE RX/TX maximum data length and time, press "bt.info", as blow, default RX/TX maximum data length is 27 and default RX/TX maxumum time is 328.
Type: LE, Role: slave, Id: 0
59:8F:3C:20:93:86 (random)
Remote address: 59:8F:3C:20:93:86 (random) (resolvable)
Local address: 80:D2:1D:E8:30:EC (public) (identity)
Remote on-air address: 59:8F:3C:20:93:86 (random) (resolvable)
Local on-air address: 7C:59:48:2E:A4:51 (random) (resolvable)
Interval: 0x0024 (45 ms)
Latency: 0x0000 (0 ms)
Supervision timeout: 0x0190 (4000 ms)
LE PHY: TX PHY LE 1M, RX PHY LE 1M
LE data len: TX (len: 27 time: 328) RX (len: 27 time: 328)
5. When LE data len is updated by the peer device, below information will be printed.
LE data len updated: TX (len: 27 time: 328) RX (len: 50 time: 512)
6. Update maximum tx data length, press "bt.data-len-update <tx_max_len> [tx_max_time]", such as bt.data-len-update 65, below information will be printed.
Calculated tx time: 632
59:8F:3C:20:93:86 (random)
data len update initiated.
LE data len updated: TX (len: 65 time: 632) RX (len: 50 time: 512)

GATT central role side,
1. Initialize the Host, press "bt.init".
2. Start scan, press "bt.scan on", Bluetooth device around your current bluetooth will be list, for example,
[DEVICE]: 7C:59:48:2E:A4:51 (random), AD evt type 4, RSSI -44 BLE Peripheral C:0 S:1 D:0 SR:1 E:0 Prim: LE 1M, Secn: No packets, Interval: 0x0000 (0 ms), SID: 0xff
3. Stop scan, press "bt.scan off",
4. Connect target device, press "bt.connect <address: XX:XX:XX:XX:XX:XX> <type: (public|random)>", such as bt.connect 7C:59:48:2E:A4:51 random
5. Check current LE RX/TX maximum data length and time, press "bt.info", as blow, default RX/TX maximum data length is 27 and default RX/TX maxumum time is 328.
Type: LE, Role: master, Id: 0
7C:59:48:2E:A4:51 (random)
Remote address: 7C:59:48:2E:A4:51 (random) (resolvable)
Local address: C0:95:DA:00:BC:82 (public) (identity)
Remote on-air address: 7C:59:48:2E:A4:51 (random) (resolvable)
Local on-air address: 59:8F:3C:20:93:86 (random) (resolvable)
Interval: 0x0024 (45 ms)
Latency: 0x0000 (0 ms)
Supervision timeout: 0x0190 (4000 ms)
LE PHY: TX PHY LE 1M, RX PHY LE 1M
LE data len: TX (len: 27 time: 328) RX (len: 27 time: 328)
6. Update maximum tx data length, press "bt.data-len-update <tx_max_len> [tx_max_time]", such as bt.data-len-update 50, below information will be printed.
Calculated tx time: 512
7C:59:48:2E:A4:51 (random)
data len update initiated.
LE data len updated: TX (len: 50 time: 512) RX (len: 27 time: 328)
7. When LE data len is updated by the peer device, below information will be printed.
LE data len updated: TX (len: 50 time: 512) RX (len: 65 time: 632)
```

- BLE GATT data signing:

```bash
GATT peripheral role side,
1. Initialize the Host, press "bt.init",
2. Advertising, press "bt.advertise on",
3. After the connection is established, perform the pairing sequence,
   it could be started from peripheral side by pressing "bt.security <level>", such as "bt.security 2",
4. After the authentication is successfully, disconnect the connection,
   it could be started from peripheral side by pressing "bt.disconnect",
5. Waiting for new connection. After the connection is established (LL enceyption should be disabled),
   add new serivce "gatt.register".

GATT central role side,
1. Initialize the Host, press "bt.init",
2. Scaning advertising packets, press "bt.scan on",
3. A few seconds later, stop the scanning, press "bt.scan off"
4. Select the target board and create a new connection. If the taregt is not listed, repeat steps 2 and 3.
   Then press "bt.connect <address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"
5. After the connection is established, perform the pairing sequence,
   it could be started from central side by pressing "bt.security <level>", such as "bt.security 2",
6. After the authentication is successfully, disconnect the connection,
   it could be started from central side by pressing "bt.disconnect",
7. Repeat the steps 2 and 3. After the connection is established (LL enceyption should be disabled),
   perform the GATT data signing sequence, press "gatt.signed-write <handle> <data> [length] [repeat]",
   such as "gatt.signed-write 22 AA 1"
```

- BLE GATT Service Changed Indication:

```bash
GATT peripheral role side,
1. Initialize the Host, press "bt.init",
2. Advertising, press "bt.advertise on",
3. After the connection is established. and waiting for the service changed indication is subsribed,
4. Add new serivce, press "gatt.register",
5. Remove the added serivce, press "gatt.unregister".

GATT central role side,
1. Initialize the Host, press "bt.init",
2. Scaning advertising packets, press "bt.scan on",
3. A few seconds later, stop the scanning, press "bt.scan off"
4. Select the target board and create a new connection. If the taregt is not listed, repeat steps 2 and 3.
   Then press "bt.connect <address: XX:XX:XX:XX:XX:XX> <type: (public|random)>"
5. After the connection is established, subscribe the GATT service changed indicator. press "bt.subscribe <CCC handle> <value handle> [ind]",
   such as "gatt.subscribe f e ind".
```
3. Zigbee test

- Forms a network
```bash
@Coex> zigbee.form
@coex> APP-EVT: Event 8, NodeState=0
APP-EVT: Request Nwk Formation 00
APP-BDB: NwkFormation Success
APP-ZDO: Network started Channel = 12
```
> **NOTE: More zigbee commands please refer [coordinator](https://bitbucket.sw.nxp.com/projects/CONNINT/repos/zigbee_public/browse/examples/zigbee_coordinator/README.md), [router](https://bitbucket.sw.nxp.com/projects/CONNINT/repos/zigbee_public/browse/examples/zigbee_router/README.md)