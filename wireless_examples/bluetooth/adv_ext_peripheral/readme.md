# Low-power Extended Advertising Peripheral

## Demo functionality overview
The application behaves as a GAP peripheral node. It enters GAP General Discoverable Mode and waits for a
GAP central node to connect and configure notifications for the temperature value.

## Implemented profile and services
The adv_ext_peripheral application implements a GATT server, a custom profile and the following services.
- Temperature Service (UUID: 01ff0200-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.1

The Temperature service is a custom service that implements the Temperature characteristic (UUID: 0x2A6E)
with a Characteristic Presentation Format descriptor (UUID: 0x2904), both defined by the Bluetooth SIG.

## Supported platforms
The following platforms support the Extended Advertising Peripheral application:
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
Also the lib_ble_host_peripheral_cm33_x.a from the *middleware\wireless\bluetooth\host\lib* should be replaced 
with the lib_ble_OPT_host_cm33_x.a from the *middleware\wireless\bluetooth\host\lib_exp*.
6.  To enable PAWR, set the gAppPAWRSupport_d define TRUE in app_preinclude.h.

## User interface
After flashing the board, both nodes enter Deep-sleep mode. To flash the board again, press **WAKESW**.
The application default configuration enables low power that disables LED support.
The user disables low power and enables LED support setting the gAppLowpowerEnabled_d define to 0.
To wake up the node, press the **WAKESW** button. The application provide guidance over the UART.

## Running the demo
The setup requires two supported platforms, one for the adv_ext_peripheral, and one for the adv_ext_central.

1. Open a serial port terminal and connect it to each platform with the settings provided in the previous paragraph.
The start screen is displayed after the board is reset.
```
Extended Advertising Application - Peripheral
Press WAKESW to see the menu
```
2. Press the **WAKESW** button. The board exits Deep-sleep mode and displays the menu.
```
 Menu
 1_Start Legacy Advertising
 2_Start Extended Scannable Advertising
 3_Start Extended Connectable Advertising
 4_Start Extended Non Connectable Non Scannable Advertising
 5_Start Periodic Advertising
Press OPTSW to choose an option
Then confirm it with the WAKESW
```
Use the **OPTSW** to choose an option. The option printed on the bottom changes every time the switch is pressed.
When the option matches your intention (for example, 3 Starts Extended Connectable Advertising), press the **WAKESW** again
to make a decision. The advertising type chosen is started and the board starts entering low-power between advertising events.
Next time the **WAKESW** is pressed, an updated menu is printed(For example, at option 3 Stop Extended Connectable Advertising).
There is no timeout for advertising. The board continues advertising until it is stopped, or a connection is established (for legacy
and extended connectable advertising only) with an adv_ext_central device.
When gAppLowpowerEnabled_d is set 0, LEDs are enabled. The ADVLED flashes whenever an
advertising starts and is ON otherwise. The CONNLED flashes whenever there is a connection under way
and is ON otherwise.
When PAWR is started , advertising data is transmitted on subevents zero and three and responses are expected on all 5 configured 
response slots of these subevents. The central application responds to the periodic data received with a six bytes data composed 
of a three bytes random number followed by a three bytes hash performed over the random number using the local IRK in the same 
fashion the RPA are generated. The advertiser prints the responses, uses the bonded devices IRKs to generate the hash over the 
random number and attempts to connect over PAWR with the responder in case the hashes match. 
```
PAWR Started
PAWR Set Subevent Data Complete
Periodic advertising response received
Adv Handle: 0
Subevent: 0
Response slot: 2
Response data: EFBE6E573231
Periodic advertising response received
Adv Handle: 0
Subevent: 3
Response slot: 2
Response data: 021CED6395B8
PAWR Set Subevent Data Complete
```

3. If the adv_ext_central connects to an adv_ext_peripheral device, it bonds (if no bond was previously made),
does service discovery (only the first time it connects with the peripheral), configures notification and waits for notifications
from the peripheral. If no data is sent within 5 seconds, the node disconnects and reenters Deep-sleep mode.
The peripheral sends a notification with the value of the temperature read through an ADC from a thermistor, if present, or randomly generated.
When the central receives the notification, it displays it on the terminal window and disconnects in 5 seconds.
```
Extended Connectable Advertising Started on handle 0
Connected!

Extended Connectable Advertising Terminated on handle 0
Disconnected!
```
When the PAWR support is enabled and a connection with a previously bonded device takes place over PAWR,
the applications behavior is similar except in this case the adv_ext_central's role is peripheral and adv_ext_peripheral's
role is central.
```
PAWR Started
PAWR Set Subevent Data Complete
Periodic advertising response received
Adv Handle: 0
Subevent: 3
Response slot: 2
Response data: 564DE1FC4957
 Gap_ConnectFromPawr Succeeded
Connected!

Disconnected!
```

