# Coex Wi-Fi UMS Open

## Overview
This application demonstrates Wi-Fi and Bluetooth LE Audio coexistence.
It combines the Wi-Fi CLI functionality with the Unicast Media Sender (UMS) LE Audio profile
using the edgefast_open BT stack.

The UMS profile acts as a unicast audio source (central) that can stream audio
to LE Audio receivers (peripherals) over 2 CIS channels.

## Supported Boards
- [MIMXRT1170-EVKB + IW612](../../_boards/evkbmimxrt1170/coex_examples/coex_wifi_ums_open/example_board_readme.md)

## Usage
After boot, the BT shell prompt appears (UMS>>). Both BT and Wi-Fi commands are available:
```
UMS>> scan              <- BT command (direct)
UMS>> connect 1         <- BT command (direct)
UMS>> play              <- BT command (direct)
UMS>> wifi wlan-version <- Wi-Fi command (with 'wifi' prefix)
UMS>> wifi wlan-scan    <- Wi-Fi command (with 'wifi' prefix)
```
