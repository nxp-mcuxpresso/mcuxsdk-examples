# hid_host

## Overview
The HID Host application implements a GATT client or server for the following profile and service:
- HID over GATT Profile v1.0
- Battery Service v1.0
- Device Information Service v1.1

The application behaves as a GAP central node. It enters the GAP Limited Discovery Procedure and searches
for HID devices to connect to. After connecting with the peripheral, it configures notifications and displays the
received HID reports on a terminal connected to the UART port. The application uses pairing with bonding by
default. When connected with the HID Device application, it sends the 999999 passcode to the host stack by
default.

## Supported Boards
- KW47-EVK
- FRDM-MCXW71
- K32W148-EVK
- MCX-W72-EVK
- KW45B41Z-EVK