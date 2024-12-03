Toolchain supported
===================
- IAR embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

User interface
================
After flashing the board, the device is in idle mode. The interaction with the board is done entirely by using the shell commands via the serial communication terminal.

Running the demo
================
The application is built to work with any other Bluetooth LE device. To showcase the functionality, two platforms are used in the following setup.
1. Open a serial port terminal and connect them to the two boards as described above. The start screen is displayed after the board is reset. All LEDs are flashing on both devices.
2. Configure one of the devices as a GAP peripheral and a Heart Rate server. Change name to HRS. Start advertising on this device.
```
BLE Shell>gap devicename Collector
--> GATTDB Event: Attribute Written
HRS>gap advdata 1 6
RS>gap advdata 8 HRS
HRS>gap advstart
HRS>
--> GAP Event: Advertising parameters successfully set.
HRS>
--> GAP Event: Advertising data successfully set.
HRS>
--> GAP Event: Advertising state changed successfully!
HRS>gattdb addservice 0x180D
--> Heart Rate
- Heart Rate Measurement Value Handle: 14
```
3. Configure the other device as a GAP central. Change its name to 'Collector'. Start scanning and connect to the HRS device by selecting the corresponding device index from the list of scanned devices. In the example below, the HRS device is device number 2. The number of listed scanned devices can be controller through the `mShellGapMaxScannedDevicesCount_c` define in `shell_gap.c`.
```
BLE Shell>gap devicename Collector
--> GATTDB Event: Attribute Written
Collector>gap scanstart filter
--> GAP Event: Scan started.
Collector>
--> GAP Event: Found device 0 : 880F102F500E 0 dBm
--> GAP Event: Found device 1 : NXP_CSCS 00049F000006 0 dBm
--> GAP Event: Found device 2 : HRS 00049F0000FF 0 dBm
Collector>gap connect 2
--> GAP Event: Scan stopped.
Collector>
--> GAP Event: Connected to peer 0
```
4. Optionally, the devices can be paired (`gAppUsePairing_d` and `gAppUseBonding_d` must be set in `app_preinclude.h`). On the collector initiate the pairing.
```
Collector>gap pair 0
--> Pairing...
--> GAP Event: Link Encrypted
--> GAP Event: Device Paired
```
5. On the Collector, start service discovery. The device discovers the GAP, GATT, and Heart Rate services.
```
Collector>gatt discover 0 -all
--> Discovered primary services: 3
--> Generic Attribute Start Handle: 1 End Handle: 4
- Service Changed Value Handle: 3
- Client Characteristic Configuration Descriptor Handle: 4
--> Generic Access Start Handle: 5 End Handle: 11
- Device Name Value Handle: 7
- Appearance Value Handle: 9
- Peripheral Preferred Connection Parameters Value Handle: 11
--> Heart Rate Start Handle: 12 End Handle: 19
- Heart Rate Measurement Value Handle: 14
- Client Characteristic Configuration Descriptor Handle: 15
- Body Sensor Location Value Handle: 17
- Heart Rate Control Point Value Handle: 19
```
6. Configure the HRS to send notifications by writing the CCCD from the Collector. Send a GATT write command with value 1 to the CCCD handle discovered, 15.
```
Collector>gatt write 0 15 0x0001
--> GATT Event: Characteristic Value Written!
```
7. Send heart rate measurement notifications from the HRS device by using the value handle obtained after adding the service in the previous step.
```
HRS>gatt notify 0 14
```
8. A notification appears on the Collector console.
```
Collector>
--> GATT Event: Received Notification
Handle: 14
Value: B400
```

### 1. Extended advertising
1. On the GAP Peripheral device:
Configure the extended advertising parameters. In the below example, the advertising type is set to connectable and includes TX power and the primary PHY is set to Coded PHY.
2. Configure the extended advertising data. The Bluetooth LE Shell applications has the feature to send for test, a large data payload. Use the extended advertisement default configuration (not call "`gap extadvcfg`"), pass the command "`gap extadvdata`" with no parameters and the default data is added. The length is configurable at compile time through SHELL_EXT_ADV_DATA_SIZE and the data pattern is
SHELL_EXT_ADV_DATA_PATTERN. Start the default test with call for "`gap extadvstart`". The advertising data type is set to shortened local name (8) and the advertising data content is set to `test_ext_adv_data`.
**_Note:_** _Users must note that extended connectable advertising does not allow for chained advertising data. The data length must be limited to what can fit in a single AUX_ADV_IND PDU (251 bytes at maximum). This means that passing the gap extadvdata with no parameters and the default value of SHELL_EXT_ADV_DATA_SIZE (500 bytes) after having set the advertising type to connectable will result in an error when trying to start advertising._
3. Start extended advertising.
```
BLE Shell>gap extadvcfg -type 65 -phy1 3
BLE Shell>gap extadvdata 8 test_ext_adv_data
BLE Shell>gap extadvstart
--> GAP Event: Extended
Advertising parameters successfully set.
--> GAP Event:
Extended Advertising data successfully set.
--> GAP Event: Advertising state changed successfully!
```
4. On the GAP Central device:
Set the scanning parameters. The scanning PHY is set to match the advertising PHY, in this case Coded PHY.
5. Start scanning and filter duplicates.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap scanstart filter
BLE Shell>
-> GAP Event: Scan started.
BLE Shell>
--> GAP Event: Found device 0 : 0060375BCEC6 -23 dBm
Advertising Extended Data:
test_ext_adv_data
```
6. Set the connection initiating PHYs corresponding to the primary PHY on which the advertising is performed.
7. Connect to the desired device in the scanned devices list.
```
BLE Shell>gap connectcfg -phy 4
--> Connection Parameters:
--> Connection Interval: 200 ms
--> Connection Latency: 0
--> Supervision Timeout: 32000 ms
--> Connecting PHYs: Coded
BLE Shell>gap connect 0
BLE Shell>
-> GAP Event: Scan stopped.
BLE Shell>
--> GAP Event: Connected to peer 0
```

### 2. RSSI Monitor
RSSI Monitor is an application that allows monitoring the RSSI of a remote peer on advertising or connection channel. The GAP peripheral device can modify the output TX power on both advertising and connection channels.
1. On GAP peripheral device
Set the primary advertising PHY to Coded PHY. Start advertising and read the address. Set the TX power in dBm to a value less than 20 dBm.
```
BLE Shell>gap address
BLE Shell>
--> GAP Event: Public Address Read:C4603770BCC5
BLE Shell>gap extadvcfg -phy1 3
BLE Shell>gap extadvstart
BLE Shell>
--> GAP Event: Extended Advertising parameters successfully set.
BLE Shell>
--> GAP Event: Extended Advertising data successfully set.
BLE Shell>
--> GAP Event: Advertising state changed successfully!
BLE Shell>gap txpower adv 0
BLE Shell>
--> GAP Event: Success!
```
2. On GAP Central device
Set the scanning PHY to Coded PHY. Start monitoring the RSSI on advertising Channel using the address of the Peripheral device. Scanning starts automatically, if it is not previously enabled.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap rssimonitor C4603770BCC5--> Reading RSSI on advertising
channel:
BLE Shell>
-> GAP Event: Scan started.
BLE Shell>
RSSI: -27 dBm
RSSI: -27 dBm
RSSI: -27 dBm
RSSI: -27 dBm
RSSI: -27 dBm
RSSI: -29 dBm
```
In the below example, the RSSI in monitored on a connection channel. On GAP Peripheral, start advertising in connectable mode on Coded PHY and adjust the TX power level.
```
BLE Shell>gap extadvcfg -type 65 -phy1 3
BLE Shell>gap extadvdata 8 rssimonitortest
BLE Shell>gap extadvstart
BLE Shell>
--> GAP Event: Extended Advertising parameters successfully set.
BLE Shell>
--> GAP Event: Extended Advertising data successfully set.
BLE Shell>
--> GAP Event: Advertising state changed successfully!
BLE Shell>
--> GAP Event: Connected to peer 0
BLE Shell>
--> GAP Event: Advertising stopped!
BLE Shell>gap txpower conn 10
BLE Shell>
--> GAP Event: Success!
```
On the GAP Central device, start scanning on the Coded PHY. Update the connection PHY also to Coded PHY, then connect to the remote device and monitor continuously the RSSI on the connection channel.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap connectcfg -phy 4
--> Connection Parameters:
--> Connection Interval: 200 ms
--> Connection Latency: 0
--> Supervision Timeout: 32000 ms
--> Connecting PHYs: Coded
BLE Shell>gap scanstart filter
BLE Shell>
-> GAP Event: Scan started.
BLE Shell>
--> GAP Event: Found device 0 : C4603770BCC5 -21 dBm
Advertising Extended Data:
rssimonitortest
gap connect 0
BLE Shell>
-> GAP Event: Scan stopped.
BLE Shell>
--> GAP Event: Connected to peer 0
BLE Shell>gap rssimonitor 0 -c
--> Reading RSSI from connected device:
BLE Shell>
RSSI: -22 dBm
RSSI: -23 dBm
RSSI: -21 dBm
RSSI: -22 dBm
RSSI: -22 dBm
BLE Shell>gap rssistop
```
3. Update the PHY preference and continue monitoring the RSSI. For coded PHY, the coding scheme can be configured between S2 and S8 (500 kbit/s and 125 kbit/s).
```
BLE Shell>gap phy 0 -tx 4 -rx 4 -o 1
BLE Shell>
--> GAP Event: Phy update complete with peer 0
--> TxPhy: Coded
--> RxPhy: Coded
BLE Shell>gap phy 0 -tx 2 -rx 2
BLE Shell>
--> GAP Event: Phy update complete with peer 0
--> TxPhy: 2M
--> RxPhy: 2M
BLE Shell>gap rssimonitor 0 -c
--> Reading RSSI from connected device:
BLE Shell>
RSSI: -23 dBm
RSSI: -23 dBm
RSSI: -21 dBm
RSSI: -21 dBm
--> GAP Event: Phy update complete with peer 0
--> TxPhy: Coded
--> RxPhy: Coded
BLE Shell>
RSSI: -22 dBm
RSSI: -21 dBm
RSSI: -22 dBm
RSSI: -23 dBm
RSSI: -23 dBm
--> GAP Event: Phy update complete with peer 0
--> TxPhy: 2M
--> RxPhy: 2M
BLE Shell>
RSSI: -22 dBm
RSSI: -21 dBm
RSSI: -21 dBm
RSSI: -20 dBm
```

### 3. Throughput feature
The Bluetooth LE Shell application also has a throughput test feature that can be used to test different combinations of the parameters (connection interval, payload size, and packet count) to determine the best data-rate.
This feature requires two devices:
- GAP Peripheral: transmits the test packets
- GAP Central: receives the packets and displays a report
All throughput-related commands are grouped under the thrput keyword:
- `thrput setparam`: configures connection interval, packet count and payload size.
- `thrput start tx`: configures the device as a GAP Peripheral and starts advertising. Once the receiving device is connected, the packet transmission begins. The packet size and count can also be specified (-s <size_value> -c <count_value>).
- `thrput start rx`: configures the device as a GAP Central and starts scanning. Once a transmitter device is found, it connects to it and waits for the test to begin. The connection interval can also be configured (-ci <value>).
- `thrput stop`: stops the test and disconnects the devices.
Once a connection is established between the devices and initial throughput test is complete, one can start a new throughput transmission test with a new set of parameters (packet size / count).
The receiving device generates the report if no packets are received for more than three consecutive connection events.
The default configuration of the throughput test is the following:
- Packet count: 1000
- Payload size: 20 bytes
- Connection interval (min, max): 160, 160 (200 ms)

The example of a test report is shown below:
```
BLE Shell>thrput start tx
BLE Shell>
--> GAP Event: Advertising parameters successfully set.
BLE Shell>
--> GAP Event: Advertising data successfully set.
BLE Shell>
--> GAP Event: Advertising started.
--> GAP Event: Connected to peer 0
BLE Shell>
--> GAP Event: Advertising Throughput test started.
Sending packets...
--> MTU Exchanged.
BLE Shell>
Throughput test with peer 0 has finished.
BLE Shell>thrput start rx
BLE Shell>
-> GAP Event: Scan started.
Found device:
THR_PER
0060375BCEC6
-> GAP Event: Scan stopped.
--> GAP Event: Connected to peer 0
BLE Shell>Throughput test started.
Receiving packets...
************************************
***** TEST REPORT FOR PEER ID 0 ****
************************************
Packets received: 1000
Total bytes: 244000
Receive duration: 5017 ms
Average bitrate: 389 kbps
************************************
********** END OF REPORT ***********
```

### 4. Decision-Based Advertising Filtering feature
The Bluetooth LE Shell application also supports the Decision-Based Advertising Filtering feature, which can be enabled by performing the following steps:
- Flash the experimental NBU found in _middleware/wireless/ble_controller/bin/experimental_.
- Update the application project so that it uses the experimental Bluetooth LE Host library, _middleware/wireless/bluetooth/host/lib_exp/lib_ble_OPT_host_cm33_iar.a_.
- In `app_preinclude.h` file, set `BLE_SHELL_DBAF_SUPPORT` to `1`.

This feature requires two devices:
- GAP Peripheral: transmits decision PDUs (ADV_DECISION_IND)
- GAP Central: scans for decision PDUs and handles filtering policies

To showcase the functionality, two platforms are used in the following setup.
Steps to perform on the GAP Peripheral device:
1. Configure the extended advertising parameters to use decision PDUs. In the below example, the advertising type is set to connectable and includes TX power, uses decision PDUs and includes AdvA in the decision PDU. The primary PHY is set to Coded PHY.
2. Configure the extended advertising data using the `gap extadvdecdata` command. The resolvable tag and/or arbitrary data can be set using the parameters available.
3. Start extended advertising.
```
BLE Shell>gap extadvcfg -phy1 3 -type 449
BLE Shell>gap extadvdecdata -key 112233445566778899AABBCCDDEEFF00 -prand 5AC317 -decdata 6362 -datalen 2 -restag 0
BLE Shell>gap extadvstart
BLE Shell>
--> GAP Event: Extended Advertising parameters successfully set.
BLE Shell>
--> GAP Event: Extended Advertising data successfully set.
BLE Shell>
--> GAP Event: Extended Advertising Decision Data Setup Complete.
BLE Shell>
--> GAP Event: Advertising state changed successfully!
BLE Shell>
```
Steps to perform on the GAP Central device:
1. Set the scanning parameters to scan only decision PDUs. The scanning PHY is set to match the advertising PHY, in this case Coded PHY.
2. Set the connection parameters to use only decision PDUs and the connection initiating PHYs corresponding to the primary PHY on which the advertising is performed.
```
BLE Shell>gap scancfg -phy 4 -filter 12
BLE Shell>gap connectcfg -phy 4 -filter 2
--> Connection Parameters:
--> Connection Interval: 200 ms
--> Connection Latency: 0
--> Supervision Timeout: 32000 ms
--> Connecting PHYs: Coded
--> Connection Filter Policy: 2
BLE Shell>
```
3. Add decision instructions using the `gap adddecinstr` command. A maximum of `gMaxNumDecisionInstructions_c` instructions can be added. If the set of instructions must be changed, the `gap deldecinstr` command deletes all current instructions.
```
BLE Shell>gap adddecinstr -group 1 -field 0 -criteria 1 -restagkey 112233445566778899AABBCCDDEEFF00
BLE Shell>gap adddecinstr -group 1 -field 6 -criteria 1 -advmode 6
BLE Shell>gap adddecinstr -group 0 -field 24 -criteria 1 -arbmask 00000000ffffff
BLE Shell>gap adddecinstr -group 0 -field 9 -criteria 1 -advacheck 2 -add1type 0 -add1 a6fb0d376000 -add2type 0 -add2 a5fb0d376000
BLE Shell>gap adddecinstr -group 0 -field 7 -criteria 1 -rssimin -80 -rssimax 0
BLE Shell>gap adddecinstr -group 0 -field 8 -criteria 5 -lossmin 0 -lossmax 50
BLE Shell>
```
4. Set the decision instructions using the `gap setdecinstr` command. The instructions are used when listening for advertisements containing decision PDUs.
5. Start scanning and filter duplicates.
6. Connect to the desired device in the scanned devices list.
```
BLE Shell>gap setdecinstr
BLE Shell>
--> GAP Event: Decision Instructions Setup Complete.
BLE Shell>gap scanstart filter
BLE Shell>
-> GAP Event: Scan started.
BLE Shell>
--> GAP Event: Found device 0 : C4603770BCC5 -23 dBm
Advertising Extended Data:
gap connect 0
BLE Shell>
-> GAP Event: Scan stopped.
BLE Shell>
--> GAP Event: Connected to peer 0
BLE Shell>
```
