Overview
========
The Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application demonstrates Generic FSK transmission/reception and Bluetooth advertising/scanning/multiple connections coexistence.

The Bluetooth LE part of this demo implements a modified version of the Wireless UART demo application, capable of multiple Bluetooth LE connections.

Based on the Hardware link-layer implementation, the Bluetooth Low Energy has a higher priority than the Generic FSK protocol and as the effect, the Generic FSK communication is executed during the Idle states (inactive periods) of the Bluetooth LE. The coexistence of the two protocols is handled internally at the Controller level.

The Bluetooth LE part of the application behaves at first as a GAP central node. It enters GAP Limited Discovery Procedure and searches for other Wireless UART devices to connect. To change the device role to GAP peripheral, use the ROLESW button. The device enters GAP General Discoverable Mode and waits for a GAP central node to connect.

The Generic FSK part of the application can either enter in the receive state by double clicking the SCANSW button or it can start the periodic transmit by long pressing the ROLESW button. It cannot enter in both states at the same time.

The Generic FSK will have lower priority than the Bluetooth LE, therefore any ongoing Generic FSK receive will be paused by the Controller when Bluetooth LE activity is ongoing and automatically resumed by the Controller when there is no Bluetooth LE activity.

The first Generic FSK transmit command will be buffered if there is continuous Bluetooth LE activity (example is continuous Bluetooth LE scanning). Any succeeding Generic FSK transmit command will indicate failure, in the command line interface, if the initial buffered transmit command was not sent yet.

This section describes the implemented profiles and services, user interactions, and testing methods for the Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application.

Implemented profile and services
================================
The Hybrid (Dual-Mode) Bluetooth Low Energy and Generic FSK application implements the GATT client and server for the custom Wireless UART profile and services and will also act as a Generic FSK transmitter/receptor, repeating a custom packet, at a fixed periodic interval, with a predefined identifier, isolated to the address used in the Bluetooth LE protocol of the demo.

• Wireless UART Service (UUID: 01ff0100-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
• Battery Service v1.0
• Device Information Service v1.1

The Wireless UART service is a custom service that implements a custom writable ASCII Char characteristic (UUID: 01ff0101-ba5e-f4ee-5ca1-eb1e5e4b1ce0) that holds the character written by the peer device.

The application is ready to start either Bluetooth LE scanning for Wireless UART Service, Bluetooth LE advertising Wireless UART Service, Generic FSK periodic transmit of a custom packet or Generic FSK receive, in the available slots not used by the Bluetooth LE protocol.


Toolchain supported
===================
- IAR embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)
- GCC ARM Embedded (ide version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- kw45b41zloc board

|   Platform   |  SCANSW  | CONNLED | ROLESW |
|:-------------|:--------:|--------:|-------:|
| KW45B41Z-LOC |    SW2   |  LED2   |   SW3  |

Board settings
==============
No special board setting.

User interface
==============
After flashing the board, the device is in idle mode (all LEDs flashing). To start Bluetooth LE scanning, press the SCANSW button. When in GAP Limited Discovery Procedure of GAP General Discoverable Mode, CONNLED is flashing. When the node connects to a peer device, CONNLED turns solid. To disconnect the node, hold the SCANSW button pressed for 2-3 seconds. The node then re-enters GAP Limited Discovery Procedure.

To start Generic FSK receiving, double click the SCANSW button and the device will start receiving packets on the same channel as the Bluetooth LE channel 37, with the network address defined in gGenFSK_NetworkAddress_c. The receiver will only print the packets that have the predefined identifier at gGenFSK_Identifier_c. To stop Generic FSK receiving double click the ROLESW button. 

On a different device, start Generic FSK transmit by long pressing the ROLESW button. To stop the periodic Generic FSK transmit long press again the ROLESW button, if the transmit procedure is ongoing. The long press ROLESW will act as a toggle for the transmit.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
5.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
The application is built to work with another supported platform running the same example. When testing with two boards, perform the following steps:

    Case 1 (Bluetooth LE):
1.  Open a serial port terminal and connect them to the two boards, in the same manner described in "Prepare the Demo".
2.  The application starts as a GAP central. To switch the role to a GAP peripheral, press the ROLESW. Depending on the role, when pressing the SCANSW, the application starts either scanning or advertising.
3.  As soon as the CONNLED turns solid on both devices, the user can start writing in one of the consoles. The text appears on the other terminal.
4.  After creating a connection, the role (central or peripheral) is displayed on the console. The role switch can be pressed again before creating a new connection.

When operating the Generic FSK mode, the following should be followed:

    Case 2 (Generic FSK):
1.  Open a serial port terminal and connect them to the two boards, in the same manner described in "Prepare the Demo".
2.  The Generic FSK communication direction is not preset. To start receiving, on one board, double click the SCANSW button and the device will start receiving packets on the same channel as the Bluetooth LE channel 37.
3.  To start transmitting, on another board, long press the ROLESW button. The transmitting device will use an identifier known by the receiver and its packets will be printed in the CLI.
4.  To stop Generic FSK receiving double click the ROLESW button.
5.  To stop the periodic Generic FSK transmit long press again the ROLESW button, if the transmit procedure is ongoing. The long press ROLESW will act as a toggle for the transmit. At this point, both devices cand reverse the direction of communication by following the exact same steps.

The application is ready to combine both scenarios. It can either establish a Bluetooth LE connection and perform Generic FSK, or during Generic FSK activity, the Bluetooth LE connection can be established, and the Controller will pause receiving or announce the discarded Generic FSK transmissions, that will be resumed after the Bluetooth LE activity is finished.

    Case 3 (Dual-Mode):
1.  Establish a Bluetooth LE connection as described in Case 1.
2.  Start Generic FSK activity as described in Case 2, independent of the Bluetooth LE roles chosen at Case 1.
3.  Start typing in either of the terminals and you can observe, as in Figure 56, that the Bluetooth LE activity is prioritized, characters will be printed in the peer’s terminal and Generic FSK will continue in the available slots, not used by the Bluetooth LE link.

Customization
=============
The steps below will help changing the default settings of this demo.

For Bluetooth LE, the default advertising config (gAdvParams) are found in the app_config.c file. Also, the scanning parameters (gScanParams) can be found in this file.

**_Note:_** The Generic FSK protocol is active during the inactive periods of the Bluetooth LE protocol. The demo is currently configured to have the scan window equal to the scan interval to make the user aware of this, this can be changed.

For Generic FSK, the following defines of interest can be found in genfsk_app.h, described in table below:

| Define | Description |
|:-----|:--------:|
| gGenFSK_NetworkAddress_c | This will be the network address used for the Generic FSK, in the transmitter payload. It is implicitly set to the 0x8E89BED6, but this can be reconfigured. Beware, it should also be changed on the receiver in the hybrid_gfsk.c controller file. |
| gGenFSK_H0Value_c        | H0 Value is used in the header. |
| gGenFSK_Identifier_c     | This is the identifier used by the transmitter to be filtered at the receiver. The current implementation filters the Generic FSK packets received, based on this define. |
| gGenFskApp_TxInterval_c  | This is the interval the transmitter will repeat the transmission of a packet. It is set in milliseconds. |

Files of interest
=================
The demo can be found in the w_uart_genfsk from the available examples.

The demo is based on the basic Wireless UART with the addition of some Generic FSK files required for working in dual-dual mode, described in table below.

| File | Description |
|:-----|:--------:|
| genfsk_app.c  | Application common module. Handles the HCI commands and events for the Generic FSK. Sends the events to the application. |
| genfsk_app.h  | Application common module. Exposes public functions. |
| hybrid_gfsk.c | Controller common module. Handles initialization of Generic FSK. |
| hybrid_gfsk.h | Controller common module. Exposes public functions. |
