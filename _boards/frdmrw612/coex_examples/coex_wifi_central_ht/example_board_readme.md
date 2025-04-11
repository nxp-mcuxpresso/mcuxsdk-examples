# 1. Overview
==============

This document provides step-by-step procedures to build and test coex examples,
and also instructions for running the included sample applications.

## 1.1 SDK
===========

- Version: NXP SDK next

- Set up NXP SDK next generation environment.

## 1.2 Hardware requirements
=============================

- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

## 1.3 Board settings
======================

No special setting to do for wifi&ble coex.

# 2. Build and flash
=====================

## 2.1 Build
=============

> flash_debug:
```bash
$ cd mcu-sdk-3.0
$ west build -b frdmrw612 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config=flash_debug -d coex_wifi_central_ht
```
> flash_release
```bash
$ cd mcu-sdk-3.0
$ west build -b frdmrw612 examples/coex_examples/coex_wifi_central_ht --toolchain armgcc --config=flash_release -d coex_wifi_central_ht
```

**NOTE:**

> 1. ```-d coex_wifi_central_ht``` -> Specify the generated project path. Can name it as needed.
> 2. Find coex_wifi_central_ht.elf/coex_wifi_central_ht.bin in coex_wifi_central_ht folder.
> 3. Only support armgcc to build coex application.

## 2.3 Flash Binaries
======================

Flash the image with the following command,

```bash

# CMD to write CPU3 coex app image to flash in J-link window:
J-Link> loadbin C:\xxx\coex_wifi_central_ht.bin, 0x08000000
```

**NOTE:**

Monolithic feature is default enabled, this means it's no need to flash binaries manually. 

If disable Wi-Fi or BLE monolithic feature, download firmware manually. 

SB firmware path: mcu-sdk-3.0/components/conn_fwloader/fw_bin

```bash
# CMD to write CPU1 wifi image to flash in J-link window:
J-Link> loadbin C:\xxx\rw61x_sb_wifi_a2.bin,0x08400000
# CMD to write CPU2 ble to flash in J-link window:
J-Link> loadbin C:\xxx\rw61x_sb_ble_a2.bin,0x08540000
```
# 3. Run

## 3.1 Prepare the Demo
========================

1. Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the example.

## 3.2 Running the example
===========================

The log below shows the output of the coex examples (based on edgefast-shell) in the terminal window:

```bash
        Coex APP
Wi-Fi cau temperature : 29
Bluetooth initialized
Advertising successfully started
STA MAC Address: C0:95:DA:01:07:36
board_type: 0, board_type mapping:
0----QFN
1----CSP
2----BGA
========================================
app_cb: WLAN: received event 12
========================================
app_cb: WLAN initialized
========================================
WLAN CLIs are initialized
========================================
ENHANCED WLAN CLIs are initialized
========================================
CLIs Available:

...

 Coex menu called
========================================
  0  Coex menu print
  w  Wifi Cli
  p  Power manager MAIN MENU
========================================

```

1. WiFi Test

> Enter 'w' to enter Wi-Fi command mode. Enter '0' to return coex menu.

- Get the Wi-Fi driver and firmware version:

```bash
>
>wlan-version
wifi commands: wlan-version
WLAN Driver Version   : v1.3.r48.p37
WLAN Firmware Version : rw610w-V2, IMU, FP99, 18.99.6.p35, PVE_FIX 1
```

- Get MAC Address:

```bash
>wlan-mac
wifi commands: wlan-mac
MAC address
STA MAC Address: C0:95:DA:01:07:36
uAP MAC Address: C2:95:DA:01:08:36
```

- Scan the network:

```bash
>wlan-scan
wifi commands: wlan-scan
Scan scheduled...

>30 networks found:
7C:10:C9:E0:D4:08  "test_ax88U" Infra
        mode: 802.11N
        channel: 1
        rssi: -60 dBm
        security: OPEN
        WMM: YES
        802.11V: YES
        802.11W: NA
04:A1:51:AB:07:1F  "netgear-2g" Infra
        mode: 802.11BG
        channel: 1
        rssi: -56 dBm
        security: WPA
        WMM: YES
        802.11W: NA
```

2. BLE Test

> NOTE:
>
> No need enter any BLE commands.
>
>The demo does not require user interaction. The application will automatically start advertising the Health Thermometer Service and it will accept the first connection request it receives. If the peer subscribes to receive temperature indications, these will be sent every 1 second. The temperature readings are simulated with values between 20 and 25 degrees Celsius.
3. Low power manager test

> Enter 'p' to enter pm command mode. Enter '0' to return coex menu.
> 
> Only support enable command now.

```bash
>enable 3 3000
```

> It may block serial port input.