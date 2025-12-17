### Overview

This document provides step-by-step procedures to build and test coex examples,
and also instructions for running the included sample applications.

### Hardware requirements
=====================
- Micro USB cable
- evkcmimxrt1060 board
- Personal Computer
- One of the following modules:
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.
  - Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

### Board settings
==============
Before building the example application define Wi-Fi module in the `_boards/evkcmimxrt1060/coex_examples/coex_wifi_a2dp_source/prj.conf`.


If you want to use Embedded Artists 2EL M.2 Module (Rev-A1), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y`

If you want to use Embedded Artists 1ZM M.2 Module (EAR00364), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1zm_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.88W8987=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1zm_m2=y`

If you want to use Embedded Artists 1XK M.2 Module (EAR00385), please set CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2 to y.
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y`
> `CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y`

### Jumper settings for RT1060-EVKC (enables external 5V supply):
========================================================
remove  J40 5-6
connect J40 1-2
connect J45 with external power(controlled by SW6)

### Murata Solution Board settings
==============================
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1060-EVKC and Murata M.2 Module in document Hardware Rework Guide for EdgeFast BT PAL.

**NOTE:**

1. To ensure that the LITTLEFS flash region has been cleaned, all flash sectors need to be erased before downloading example code.
2. After downloaded binary into qspiflash and boot from qspiflash directly, please reset the board by pressing SW7 or power off and on the board to run the application.

### Build and flash

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
$ west build -b evkcmimxrt1060 examples/coex_examples/coex_wifi_a2dp_source --toolchain armgcc --config flexspi_nor_debug -d coex_wifi_a2dp_source
```

> flexspi_nor_release:
```bash
$ cd <sdk root>
$ west build -b evkcmimxrt1060 examples/coex_examples/coex_wifi_a2dp_source --toolchain armgcc --config flexspi_nor_release -d coex_wifi_a2dp_source
```

**NOTE:**

> 1. ```-d coex_wifi_a2dp_source``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_a2dp_source.elf/coex_wifi_a2dp_source.bin in coex_wifi_a2dp_source folder.
> 3. Only support armgcc to build coex application.

### 4. Flash Binaries

Flash the image with the following command,

```bash
# CMD to write CPU3 coex app image to flash in J-link window:
J-Link> loadbin C:\xxx\coex_wifi_a2dp_source.bin, 0x60000000
```

### Run

### Prepare the Demo

1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J53) on the board.
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
     Initialize Firecrest-2EL (IW612) M2 Module
========================================
Download BT FW...
download starts(429436)
...................................................................................................................................................................................................................................................................................................................................................................................................................................
download success!
Download WLAN FW...
STA MAC Address: A0:CD:F3:77:E3:D8
supplicant_main_task: 632 Starting wpa_supplicant thread with debug level: 6

host init done
========================================
app_cb: WLAN: received event 12
========================================
app_cb: WLAN initialized
========================================
WLAN CLIs are initialized
========================================
CLIs Available:
========================================

help (exit: 0)
wlan-version
wlan-mac
wlan-thread-info
wlan-net-stats
wlan-set-mac <MAC_Address>
wlan-scan
wlan-scan-opt ssid <ssid> bssid ...
wlan-add <profile_name> ssid <ssid> bssid...
wlan-remove <profile_name>
wlan-list
wlan-connect <profile_name>
wlan-connect-opt <profile_name> ...
wlan-reassociate
wlan-start-network <profile_name>
wlan-stop-network
wlan-disconnect
wlan-stat
wlan-info
wlan-address
wlan-uap-disconnect-sta <mac address>
wlan-get-uap-channel
wlan-get-uap-sta-list
wlan-uap-set-beacon-interval
wlan-uap-get-beacon-interval
wlan-ieee-ps <0/1>
wlan-set-ps-cfg <null_pkt_interval>
wlan-deep-sleep-ps <0/1>
wlan-get-beacon-interval
wlan-get-ps-cfg
wlan-set-max-clients-count <max clients count>
wlan-get-max-clients-count
wlan-rts <sta/uap> <rts threshold>
wlan-frag <sta/uap> <fragment threshold>
wlan-host-11k-enable <0/1>
wlan-host-11k-neighbor-req [ssid <ssid>]
wlan-host-11v-bss-trans-query <0..16>
wlan-mbo-nonprefer-ch "<oper_class>:<chan>:<preference>:<reason> <oper_class>:<chan>:<preference>:<reason>"
wlan-mbo-set-cell-capa <cell capa: 1/2/3(default)>
wlan-mbo-set-oce <oce: 1(default)/2>
wlan-set-okc <okc: 0(default)/1>
wlan-pmksa-list
wlan-pmksa-flush
wlan-set-scan-interval <scan_int: in seconds>
wlan-sta-filter  <filter mode> [<mac address list>]
wlan-get-log <sta/uap> <ext>
wlan-roaming <0/1> <rssi_threshold>
wlan-send-hostcmd
wlan-ext-coex-uwb
wlan-set-uap-bandwidth <1/2/3> 1:20 MHz 2:40MHz 3:80MHz
wlan-set-uap-hidden-ssid <0/1/2>
wlan-eu-crypto-rc4 <EncDec>
wlan-eu-crypto-aes-wrap <EncDec>
wlan-eu-crypto-aes-ecb <EncDec>
wlan-eu-crypto-ccmp-128 <EncDec>
wlan-eu-crypto-ccmp-256 <EncDec>
wlan-eu-crypto-gcmp-128 <EncDec>
wlan-eu-crypto-gcmp-256 <EncDec>
wlan-ft-roam <bssid> <channel>
wlan-set-antcfg <ant mode> [evaluate_time]
wlan-get-antcfg
wlan-scan-channel-gap <channel_gap_value>
wlan-wmm-stat <bss_type>
wlan-reset
wlan-set-regioncode <region-code>
wlan-get-regioncode
wlan-11d-enable <sta/uap> <0/1>
wlan-uap-set-ecsa-cfg <block_tx> <oper_class> <new_channel> <switch_count> <bandwidth>
wlan-txrx-histogram <action> <enable>
wlan-uapsd-enable <uapsd_enable>
wlan-uapsd-qosinfo <qos_info>
wlan-uapsd-sleep-period <sleep_period>
wlan-generate-wps-pin
wlan-start-wps-pbc
wlan-start-wps-pin <8 digit pin>
wlan-wps-cancel
wlan-start-ap-wps-pbc
wlan-start-ap-wps-pin <8 digit pin>
wlan-wps-ap-cancel
wlan-get-signal
wlan-set-bandcfg
wlan-get-bandcfg
wlan-enable-disable-htc <option>
wlan-set-su <0/1>
wlan-get-turbo-mode <STA/UAP>
wlan-set-turbo-mode <STA/UAP> <mode>
wlan-set-multiple-dtim <value>
wlan-cloud-keep-alive <start/stop/reset>
wlan_tcp_client dst_ip <dst_ip> src_port <src_port> dst_port <dst_port>
wlan-set-country <country_code_str>
wlan-set-country-ie-ignore <0/1>
wlan-set-indrstcfg <mode> <gpio_pin>
wlan-get-indrstcfg
wlan-independent-reset <mode>
wlan-get-channel-load <set/get> <duration>
ping [-s <packet_size>] [-c <packet_count>] [-W <timeout in sec>] <ipv4/ipv6 address>
iperf [-s|-c <host>|-a|-h] [options]
dhcp-stat
========================================
========================================
app_cb: WLAN: received event 16
========================================
app_cb: WLAN: PS_ENTER
========================================
app_cb: WLAN: received event 16
========================================
app_cb: WLAN: PS_ENTER
Bluetooth A2dp Source demo start...
Bluetooth initialized
BR/EDR set connectable and discoverable done

Copyright  2024  NXP

WiFi shell initialization


BLE shell initialization

@Coex>
```
1. WiFi Test

> NOTE: All wifi commands require adding `wifi` prefix.

- Get the Wi-Fi driver and firmware version:

```bash
@Coex> wifi wlan-version
WLAN Driver Version   : v1.3.r48.p53
@Coex> WLAN Firmware Version : w9177o-V1, SDIO, FP99, 18.99.3.p27.8, PVE_FIX 1
Command wlan-version executed
```

- Get MAC Address:

```bash
@Coex> wifi wlan-mac
MAC address
@Coex> STA MAC Address: A0:CD:F3:77:E3:D8
uAP MAC Address: A2:CD:F3:77:E4:D8
Command wlan-mac executed
```

- Scan the network:

```bash
@Coex> wifi wlan-scan
Scan scheduled...
@Coex> Command wlan-scan executed
2 networks found:
7C:21:0E:17:B3:80  "NXP" Infra
        mode: 802.11N
        channel: 1
        rssi: -66 dBm
        security: WPA2 Enterprise
        WMM: YES
        802.11W: NA
        WPS: NO
7C:21:0E:17:B3:86  "NXPOPEN" Infra
        mode: 802.11N
        channel: 1
        rssi: -67 dBm
        security: WPA2
        WMM: YES
        802.11K: YES
        802.11V: YES
        802.11W: NA
        WPS: NO
```

2. BR/EDR Test

> NOTE: Please use the command "help" to view the specific commands supported by the example.

- start to find BT devices:

```bash
@Coex> bt discover on
Discovery started. Please wait ...
@Coex> BR/EDR discovery complete
[1]: 48:74:12:3F:9E:04, RSSI -66 OnePlus Nord CE 2
[2]: 50:84:92:56:F9:54, RSSI -89 NXL37690
[3]: A0:80:69:F9:14:1E, RSSI -72 SMW003174
[4]: C1:A1:B2:FE:B3:1C, RSSI -92 CB-ARMOUR
```
- connect to the device that is found, for example: bt connectdevice n (from 1):

```bash
@Coex> bt connect 1
Connection pending
@Coex> SDP discovery started
Connected
sdp success callback
A2DP Service found. Connecting ...
Security changed: 04:21:44:03:57:9F level 2
a2dp connected success
a2dp start playing
```
> After connection established, Bluetooth A2dp Source demo will start playing audio.

- disconnect current connection

```bash
@Coex> bt disconnect
@Coex> Disconnection success
```

- delete all devices:

```bash
@Coex> bt delete
@Coex> success
```
> Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.