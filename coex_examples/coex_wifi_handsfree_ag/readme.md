# coex_wifi_handsfree_ag

## Overview
This example demonstrates the coexistence of the Bluetooth Hands-Free Profile (HFP)
Audio Gateway (AG) role (edgefast_open Bluetooth stack) running simultaneously with Wi-Fi.
The AG plays the role of the "phone": it discovers and connects to a Hands-Free (HF) unit
- for example a headset or the companion `coex_wifi_handsfree` example - and originates/
manages calls (start incoming call, open/close audio, accept/reject, 3-way calling),
negotiates the SCO codec (CVSD / mSBC) and carries bidirectional SCO voice audio, while
Wi-Fi stays connected and can run data traffic (scan / ping / iperf) at the same time.

The application is built on the shared `middleware/wireless/coex` glue with independent
NB (BT) firmware download over UART followed by WLAN initialization over SDIO. A single
fsl_shell prompt (`@Coex>`) exposes both a `bt` command (HFP-AG control) and a `wifi`
command (Wi-Fi CLI dispatch). HFP SCO audio is routed through the on-board WM8962 codec
(SAI1), with the module PCM link on SAI3.

This document provides step-by-step procedures to build and test the example, and also
instructions for running the included sample application.

## Supported Boards
- [MIMXRT1170-EVKB + IW612 / IW416](../../_boards/evkbmimxrt1170/coex_examples/coex_wifi_handsfree_ag/example_board_readme.md)
