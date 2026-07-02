# Coex Wi-Fi Broadcast Media Receiver Open

## Overview
This application demonstrates Wi-Fi + BLE LE Audio coexistence using the edgefast_open
Broadcast Media Receiver (BMR) example combined with Wi-Fi CLI functionality.

The app receives LE Audio broadcast streams (LC3 encoded) and plays them through the
WM8962 audio codec while simultaneously providing Wi-Fi connectivity.

## Architecture
- Uses combo firmware (BT+Wi-Fi loaded together over SDIO)
- BT task runs broadcast_media_receiver_task with its own shell
- Wi-Fi commands are registered on the same shell with "wifi" prefix

## Supported Boards
- evkbmimxrt1170 (CM7) with IW612 (Murata 2EL M2)

## Shell Commands
After boot, the BT shell prompt appears. Both BT and Wi-Fi commands are available:
```
BMR>> scan                       <- Scan for broadcast sources
BMR>> sync <id>                  <- Sync to a broadcast source
BMR>> wifi wlan-version          <- Wi-Fi command
BMR>> wifi wlan-scan             <- Wi-Fi scan
```

## Build
```bash
west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_bmr_open \
  -d build/coex_wifi_bmr_open --config flexspi_nor_release \
  -- -Dcore_id=cm7 -DCONFIG_TOOLCHAIN=armgcc
```
