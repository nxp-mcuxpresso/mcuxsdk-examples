# Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK

The Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application demonstrates Generic FSK transmission/reception and Bluetooth advertising/scanning/multiple connections coexistence.

The Bluetooth LE part of this demo implements a modified version of the Wireless UART demo application, capable of multiple Bluetooth LE connections.

Based on the Hardware link-layer implementation, the Bluetooth Low Energy has a higher priority than the Generic FSK protocol and as the effect, the Generic FSK communication is executed during the Idle states (inactive periods) of the Bluetooth LE. The coexistence of the two protocols is handled internally at the Controller level.

The Bluetooth LE part of the application behaves at first as a GAP central node. It enters GAP Limited Discovery Procedure and searches for other Wireless UART devices to connect. To change the device role to GAP peripheral, use the **ROLESW** button. The device enters GAP General Discoverable Mode and waits for a GAP central node to connect.

The Generic FSK part of the application can either enter in the receive state by double clicking the **SCANSW** button or it can start the periodic transmit by long pressing the **ROLESW** button. It cannot enter in both states at the same time.

The Generic FSK will have lower priority than the Bluetooth LE, therefore any ongoing Generic FSK receive will be paused by the Controller when Bluetooth LE activity is ongoing and automatically resumed by the Controller when there is no Bluetooth LE activity.

The first Generic FSK transmit command will be buffered if there is continuous Bluetooth LE activity (example is continuous Bluetooth LE scanning). Any succeeding Generic FSK transmit command will indicate failure, in the command line interface, if the initial buffered transmit command was not sent yet.

This section describes the implemented profiles and services, user interactions, and testing methods for the Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application.

## Implemented profile and services
The Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application implements the GATT client and server for the custom Wireless UART profile and services and will also act as a Generic FSK transmitter/receptor, repeating a custom packet, at a fixed periodic interval, with a predefined identifier, isolated to the address used in the Bluetooth LE protocol of the demo.

- Wireless UART Service (UUID: 01ff0100-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.1

The Wireless UART service is a custom service that implements a custom writable ASCII Char characteristic (UUID: 01ff0101-ba5e-f4ee-5ca1-eb1e5e4b1ce0) that holds the character written by the peer device.

The application is ready to start either Bluetooth LE scanning for Wireless UART Service, Bluetooth LE advertising Wireless UART Service, Generic FSK periodic transmit of a custom packet or Generic FSK receive, in the available slots not used by the Bluetooth LE protocol.

## Customization
The steps below will help changing the default settings of this demo.

For Bluetooth LE, the default advertising config (`gAdvParams`) are found in the app_config.c file. Also, the scanning parameters (`gScanParams`) can be found in this file.

**_Note:_** _The Generic FSK protocol is active during the inactive periods of the Bluetooth LE protocol. The demo is currently configured to have the scan window equal to the scan interval to make the user aware of this, this can be changed._

For Generic FSK, the following defines of interest can be found in genfsk_app.h, described in table below:

| Define | Description     |
|:-------|:----------------|
| gGenFSK_NetworkAddress_c | This will be the network address used for the Generic FSK, in the transmitter payload. It is implicitly set to the 0x8E89BED6, but this can be reconfigured. Beware, it should also be changed on the receiver in the hybrid_gfsk.c controller file. |
| gGenFSK_H0Value_c        | H0 Value is used in the header. |
| gGenFSK_Identifier_c     | This is the identifier used by the transmitter to be filtered at the receiver. The current implementation filters the Generic FSK packets received, based on this define. |
| gGenFskApp_TxInterval_c  | This is the interval the transmitter will repeat the transmission of a packet. It is set in milliseconds. |

## Files of interest
The demo can be found in the w_uart_genfsk from the available examples.

The demo is based on the basic Wireless UART with the addition of some Generic FSK files required for working in dual-dual mode, described in table below.

| File | Description |
|:-----|:------------|
| genfsk_app.c       | Application common module. Handles the HCI commands and events for the Generic FSK. Sends the events to the application. |
| genfsk_app.h       | Application common module. Exposes public functions. |
| hybrid_gfsk.c      | Controller common module. Handles initialization of Generic FSK. |
| hybrid_gfsk.h      | Controller common module. Exposes public functions. |


## Supported Boards
The Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application is supported on the following platform:
- KW45B41Z-EVK
- KW45B41Z-LOC
