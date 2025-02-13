# Low-power Extended Advertising Central

## Demo functionality overview
The application behaves as a GAP central node. It enters GAP Limited Discovery Procedure and searches
for peripherals devices to pair with. After pairing with the peripheral, it configures notifications and displays
temperature values on a terminal connected to the UART port.

## Implemented profile and services
The adv_ext_central application implements a GATT client or server for the following profile and services.
- Temperature Service (UUID: 01ff0200-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.1

The Temperature service is a custom service that implements the Temperature characteristic (UUID: 0x2A6E)
with a Characteristic Presentation Format descriptor (UUID: 0x2904), both defined by the Bluetooth SIG.

## Supported platforms
The following platforms support the Extended Advertising Central application:
- KW45B41Z-EVK
- K32W148-EVK
- FRDM-MCXW71
- MCX-W71-EVK
- KW47-EVK
- MCX-W72-EVK
- FRDM-MCXW72

## Prepare the Demo
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  The PAWR usage requires the experimental NBU image from *middleware\wireless\ble_controller\bin* to be downloaded. 
Also the lib_ble_host_central_cm33_x.a from the *middleware\wireless\bluetooth\host\lib* should be replaced 
with the lib_ble_OPT_host_cm33_x.a from the *middleware\wireless\bluetooth\host\lib_exp*.
6.  To enable PAWR, set the gAppPAWRSupport_d define TRUE in app_preinclude.h.
7.  In case PAWR is enabled and more than one ext_adv_centrals are to be synced with the PAWR train, a different
response slot may be configured for each board using the gUserDefinedResponseSlot_c define in app_preinclude.h 
to avoid collisions. Otherwise, a random response slot will be chosen by the application after reset.

## User interface
After flashing the board, the application enters Deep-sleep mode. To flash the board again, press **WAKESW**.
The application default configuration enables low power that disables LED support.
The user disables low power and enables LED support setting the gAppLowpowerEnabled_d define to 0.
To wake up the node, press the **WAKESW** button. The application provide guidance over the UART.

## Running the demo
The setup requires two supported platforms, one for the adv_ext_peripheral, and one for the adv_ext_central.

1. Open a serial port terminal and connect it to each platform with the settings provided in the previous paragraph.
The start screen is displayed after the board is reset.
```
Extended Advertising Application - Central
Press WAKESW to Start Active Scanning!
Press WAKESW Long to Start Passive Scanning!
```
2. The adv_ext_central application exposes two options: Press **WAKESW** to start
active scanning or long press **WAKESW** to start passive scanning. If catching extended scannable advertising is
not an option, choose passive scanning. Otherwise, select active scanning.
The device wakes up, starts scanning, and enters Deep-sleep mode. The scanning ends when the 60 seconds timeout is
reached or when a connection with an adv_ext_peripheral device is established.
During scanning, all advertisements caught from adv_ext_peripheral devices are displayed on the terminal
window. When an extended non-connectable, non-scannable advertising with a periodic advertising attached is detected,
the adv_ext_central device attempts to sync with the periodic advertising train and prints the periodic advertising
data on the terminal window.
```
Passive Scanning Started

Extended Advertising Found
Adv Properties:
Non Connectable
Non Scannable
Undirected
Adv Data
Extended Advertising
Adv Address C4603770BCC5
Data Set Id = 4
PrimaryPHY = gLePhyCoded_c
SecondaryPHY = gLePhyCoded_c
periodicAdvInterval = 2400
Adv Data
EA Non Connectable Non Scanable DataId1 01 EA Non Connectable Non Scanable DataId1 02
EA Non Connectable Non Scanable DataId1 03 EA Non Connectable Non Scanable DataId1 04
Gap_PeriodicAdvCreateSync Succeded
Periodic Adv Sync Established
Periodic Adv Found
syncHandle = 80
Periodic Data
EA Periodic Data Id1 01 EA Periodic Data Id1 02 EA Periodic Data Id1 03 EA Periodic Data Id1 04
EA Periodic Data Id1 05 EA Periodic Data Id1 06 EA Periodic Data Id1 07 EA Periodic Data Id1 08
EA Periodic Data Id1 11 EA Periodic Data Id1 12 EA Periodic Data Id1 13 EA Periodic Data Id1 14
EA Periodic Data Id1 15 EA Periodic Data Id1 16 EA Periodic Data Id1 17 EA Periodic Data Id1 18
Stop Scanning

Periodic Adv Sync Terminated
```
If the PAWR support is enabled and the periodic advertising train the adv_ext_central has synced to is with responses
the application attempts to sync with the subevents zero and three, periodic advertising data printed being slightly different.
 Also, the application responds to the periodic data received with a six bytes data composed of a three bytes random number 
 followed by a three bytes hash performed over the random number using the local IRK in the same fashion the RPA are generated. 
 The advertiser uses the bonded devices IRKs to generate the hash over the random number and attempts to connect over PAWR with 
 the responder in case the hashes match. 

```
Passive Scanning Started

Extended Advertising Found
Adv Properties:
Non Connectable
Non Scannable
Undirected
Adv Data
Extended Advertising
Adv Address C4603770BCC5
Data Set Id = 4
PrimaryPHY = gLePhyCoded_c
SecondaryPHY = gLePhyCoded_c
periodicAdvInterval = 2400
Adv Data
EA Non Connectable Non Scanable DataId1 01 EA Non Connectable Non Scanable DataId1 02
EA Non Connectable Non Scanable DataId1 03 EA Non Connectable Non Scanable DataId1 04
Gap_PeriodicAdvCreateSync Succeded
Periodic Adv Sync Established
Gap_SetPeriodicSyncSubevent Succeded
PAWR received
Sync Handle: 0050
Event Counter: 0007
Subevent: 0
Subevent data:
PAWR Data0 For Subevent 0
PAWR Data1 For Subevent 0
Gap_SetPeriodicAdvResponseData Succeded
Periodic Adv Set Response Data Complete
```
When the 60 seconds timer expires or the connection ends, the device reenters Deep-sleep mode until the **WAKESW** is pressed
again and all syncs with periodic advertising trains are terminated.
If gAppLowpowerEnabled_d is set 0, LEDs are enabled. The SCANLED flashes, whenever the device is scanning and is ON otherwise.
The CONNLED flashes, whenever there is a connection under way and is ON otherwise.
3. If the adv_ext_central connects to an adv_ext_peripheral device, it bonds (if no bond was previously made),
does service discovery (only the first time it connects with the peripheral), configures notification and waits for notifications
from the peripheral. If no data is sent within 5 seconds, the node disconnects and reenters Deep-sleep mode.
The peripheral sends a notification with the value of the temperature read through an ADC from a thermistor, if present, or randomly generated.
When the central receives the notification, it displays it on the terminal window and disconnects in 5 seconds.
```
Passive Scanning Started

Extended Advertising Found
Adv Properties:
Connectable
Non Scannable
Undirected
Adv Data
Extended Advertising
Adv Address C4603770BCC5
Data Set Id = 3
PrimaryPHY = gLePhyCoded_c
SecondaryPHY = gLePhyCoded_c
periodicAdvInterval = 0
Adv Data
EA Connectable Data 01 EA Connectable Data 02 EA Connectable Data 03
EA Connectable Data 04 EA Connectable Data 05 EA Connectable Data 06
EA Connectable Data 07 EA Connectable Data 08 EA Connectable Data 09
Found device:
EA*PRPHC4603770BCC5
Connected!
Temperature: 24 C

Disconnected!
```
When the PAWR support is enabled and a connection with a previously bonded device takes place over PAWR,
the applications behavior is similar except in this case the adv_ext_central's role is peripheral and adv_ext_peripheral's
role is central.
```
PAWR received
Sync Handle: 0050
Event Counter: 0007
Subevent: 0
Subevent data:
PAWR Data0 For Subevent 0
PAWR Data1 For Subevent 0
Gap_SetPeriodicAdvResponseData Succeded
Periodic Adv Set Response Data Complete

Connected!
Temperature: 24 C

Disconnected!
```

