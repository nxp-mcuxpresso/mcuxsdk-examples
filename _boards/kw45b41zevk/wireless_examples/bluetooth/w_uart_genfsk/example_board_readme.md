Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
- Personal Computer

Hardware references
=====================
|   Platform   |  SCANSW  | CONNLED | ROLESW |
|:-------------|:--------:|:-------:|:------:|
| KW45B41Z-EVK |    SW2   |  LED2   |   SW3  |

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
==============
After flashing the board, the device is in idle mode (all LEDs flashing). To start Bluetooth LE scanning, press the **SCANSW** button. When in GAP Limited Discovery Procedure of GAP General Discoverable Mode, **CONNLED** is flashing. When the node connects to a peer device, **CONNLED** turns solid. To disconnect the node, hold the **SCANSW** button pressed for 2-3 seconds. The node then re-enters GAP Limited Discovery Procedure.

To start Generic FSK receiving, double click the **SCANSW** button and the device will start receiving packets on the same channel as the Bluetooth LE channel 37, with the network address defined in `gGenFSK_NetworkAddress_c`. The receiver will only print the packets that have the predefined identifier at `gGenFSK_Identifier_c`. To stop Generic FSK receiving double click the **ROLESW** button. 

On a different device, start Generic FSK transmit by long pressing the **ROLESW** button. To stop the periodic Generic FSK transmit long press again the **ROLESW** button, if the transmit procedure is ongoing. The long press **ROLESW** will act as a toggle for the transmit.

Running the demo
================
The application is built to work with another supported platform running the same example. When testing with two boards, perform the following steps:

    Case 1 (Bluetooth LE):
1.  Open a serial port terminal and connect them to the two boards, in the same manner described in "Prepare the Demo".
2.  The application starts as a GAP central. To switch the role to a GAP peripheral, press the **ROLESW**. Depending on the role, when pressing the **SCANSW**, the application starts either scanning or advertising.
3.  As soon as the **CONNLED** turns solid on both devices, the user can start writing in one of the consoles. The text appears on the other terminal.
4.  After creating a connection, the role (central or peripheral) is displayed on the console. The role switch can be pressed again before creating a new connection.

When operating the Generic FSK mode, the following should be followed:

    Case 2 (Generic FSK):
1.  Open a serial port terminal and connect them to the two boards, in the same manner described in "Prepare the Demo".
2.  The Generic FSK communication direction is not preset. To start receiving, on one board, double click the **SCANSW** button and the device will start receiving packets on the same channel as the Bluetooth LE channel 37.
3.  To start transmitting, on another board, long press the **ROLESW** button. The transmitting device will use an identifier known by the receiver and its packets will be printed in the CLI.
4.  To stop Generic FSK receiving double click the **ROLESW** button.
5.  To stop the periodic Generic FSK transmit long press again the **ROLESW** button, if the transmit procedure is ongoing. The long press **ROLESW** will act as a toggle for the transmit. At this point, both devices cand reverse the direction of communication by following the exact same steps.

The application is ready to combine both scenarios. It can either establish a Bluetooth LE connection and perform Generic FSK, or during Generic FSK activity, the Bluetooth LE connection can be established, and the Controller will pause receiving or announce the discarded Generic FSK transmissions, that will be resumed after the Bluetooth LE activity is finished.

    Case 3 (Dual-Mode):
1.  Establish a Bluetooth LE connection as described in Case 1.
2.  Start Generic FSK activity as described in Case 2, independent of the Bluetooth LE roles chosen at Case 1.
3.  Start typing in either of the terminals and you can observe, that the Bluetooth LE activity is prioritized, characters will be printed in the peer’s terminal and Generic FSK will continue in the available slots, not used by the Bluetooth LE link.
