## Overview
This application implements the central role. It:
1. Scans for the health_care_iot_peripheral device or the health_care_iot_hub device.
2. Connects to it
3. Pairs with it
4. Enables temperature indications
5. Prints the reported temperature each time an indication is received

## Running the demo
1. The central app will now autonomously scan.
2. The central app will only connect to nearby devices with a RSSI value > PROXIMITY_RSSI_THRESHOLD
3. Once a connection is set up with a nearby device, pairing procedure is automatically initiated
4. The output of the central app looks like:

Using bdAddr from NMPA
bd_addr = 44:d4:65:09:e1:86
Supply mode set to: DC/DC HV supplied (buck)
Start scanning for peripheral
Scanning on
New device: b4:10:7a:32:12:6e
New device: b4:10:7a:32:12:6e
New device: 34:82:98:28:ad:5d
New device: 34:82:98:28:ad:5d
New device: e8:42:36:12:35:a8
New device: ba:9b:31:df:c3:1
New device: 24:9a:3e:c5:d4:26
New device: b7:d2:18:5a:76:15
New device: 71:bb:bd:cb:73:7d
New device: 76:3c:8c:fc:a9:3d
New device: 95:75:1f:a:3a:3
New device: cf:4c:a1:40:f4:57
  Service: 0x1809
NAME: 'HCIOT_PERIPH_E6C2' rssi: -36, rssi threshold: -42
Device found!
New device: 33:72:c4:1a:e9:59
New device: 33:72:c4:1a:e9:59
Scanning off
Connecting...
Connected to peer device
MTU changed
5 services discovered
Health thermometer service found
4 health thermometer characteristics discovered
Health thermometer temperature characteristic found
1 descriptors found
Temperature client characteristic configuration descriptor found
Privilege error, pairing with device id 0...
Link is encrypted. Retrying temperature indication setup
Successfully paired
Temperature indication received @0s: 22
Temperature indication received @1s: 22
Temperature indication received @2s: 22

#### Supported boards:
- FRDM-MCXW23
- MCXW23-EVK

## Supported Boards
- [MCXW23-EVK](../../../_boards/mcxw23evk/wireless_examples/reference_design/health_care_iot_central/example_board_readme.md)
- [FRDM-MCXW23](../../../_boards/frdmmcxw23/wireless_examples/reference_design/health_care_iot_central/example_board_readme.md)