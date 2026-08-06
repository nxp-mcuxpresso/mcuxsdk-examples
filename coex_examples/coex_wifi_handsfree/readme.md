# coex_wifi_handsfree

## Overview
This example demonstrates the coexistence of the Bluetooth Hands-Free Profile (HFP)
Hands-Free (HF) unit role (edgefast_open Bluetooth stack) running simultaneously with
Wi-Fi. The HF unit connects to an Audio Gateway (AG) - for example a phone or the
companion `coex_wifi_handsfree_ag` example - and handles call control (answer/reject/
dial), volume, codec negotiation (CVSD / mSBC) and bidirectional SCO voice audio, while
Wi-Fi stays connected and can run data traffic (scan / ping / iperf) at the same time.

The application is built on the shared `middleware/wireless/coex` glue with independent
NB (BT) firmware download over UART followed by WLAN initialization over SDIO. A single
fsl_shell prompt (`@Coex>`) exposes both a `bt` command (HFP-HF control) and a `wifi`
command (Wi-Fi CLI dispatch). HFP SCO audio is routed through the on-board WM8962 codec
(SAI1), with the module PCM link on SAI3.

This document provides step-by-step procedures to build and test the example, and also
instructions for running the included sample application.

## Supported Boards
- [MIMXRT1170-EVKB + IW612 / IW416](../../_boards/evkbmimxrt1170/coex_examples/coex_wifi_handsfree/example_board_readme.md)
