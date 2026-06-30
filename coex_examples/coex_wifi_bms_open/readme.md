# Coex Wi-Fi Broadcast Media Sender Open

## Overview
This application demonstrates Wi-Fi + BLE LE Audio coexistence using the edgefast_open
Broadcast Media Sender (BMS) example combined with Wi-Fi CLI functionality.

The app broadcasts LE Audio (LC3 encoded) from a WAV file on USB mass storage while
simultaneously providing Wi-Fi connectivity for network operations.

## Architecture
- Uses combo firmware (BT+Wi-Fi loaded together over SDIO)
- BT task runs broadcast_media_sender_task with its own shell
- Wi-Fi commands are registered on the same shell with "wifi" prefix

## Supported Boards
- evkbmimxrt1170 (CM7) with IW612 (Murata 2EL M2)

## Shell Commands
After boot, the BT shell prompt appears. Both BT and Wi-Fi commands are available:
```
BMS>> wav_open 1:/audio.wav    <- Open WAV file from USB
BMS>> lc3_preset 48_2_1        <- Select LC3 preset
BMS>> wifi wlan-version         <- Wi-Fi command
BMS>> wifi wlan-scan            <- Wi-Fi scan
```

## Build
```bash
west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_bms_open \
  -d build/coex_wifi_bms_open --config flexspi_nor_release \
  -- -Dcore_id=cm7 -DCONFIG_TOOLCHAIN=armgcc
```
