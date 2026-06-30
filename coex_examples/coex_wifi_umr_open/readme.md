# Coex Wi-Fi UMR Open

## Overview
This application demonstrates Wi-Fi and Bluetooth LE Audio coexistence.
It combines the Wi-Fi CLI functionality with the Unicast Media Receiver (UMR) LE Audio profile
using the edgefast_open BT stack.

The UMR profile acts as a unicast audio sink (peripheral) that receives audio
from LE Audio senders (centrals) over 1 CIS channel.

**Note:** This app includes the CAAM DMA workaround (`--wrap=bt_encrypt_le`) to prevent
BT controller unresponsiveness caused by CAAM hardware DMA conflicting with BT UART DMA
when CSIP RSI generation calls `bt_encrypt_le()`.

## Supported Boards
- [MIMXRT1170-EVKB + IW612](../../_boards/evkbmimxrt1170/coex_examples/coex_wifi_umr_open/example_board_readme.md)

## Usage
After boot, the BT shell prompt appears (UMR>>). Both BT and Wi-Fi commands are available:
```
UMR>> advertise         <- BT command (direct)
UMR>> wifi wlan-version <- Wi-Fi command (with 'wifi' prefix)
UMR>> wifi wlan-scan    <- Wi-Fi command (with 'wifi' prefix)
```
