Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- k32w148evk board

Hardware references
=====================

| Platform        | SCANSW      | CONNLED  | ROLESW  |
| --------------- | ----------- | -------- | ------- |
| K32W148-EVK     | SW2         | LED2     | SW3     |

Board settings
==============
No special board setting.

Prepare the Demo
================
Open a serial terminal with the following settings:
- 115200 baud rate
- 8 data bits
- No parity
- One stop bit
- No flow control

User interface
================
After flashing the board, the device is in idle mode (all LEDs
flashing). To start scanning, press the  **SCANSW** button. When in GAP
Limited Discovery Procedure of GAP General Discoverable Mode,
**CONNLED** is flashing. When the node connects to a peer device,
**CONNLED** turns solid. To disconnect the node, hold the  **SCANSW**
button pressed for 2-3 seconds. The node then re-enters GAP Limited
Discovery Procedure.

Running the demo
================
The application is built to work with another supported platform
running the same example or with the Wireless UART from the IoT
Toolbox application.
- When testing with two boards, perform the
following steps:

    1.  Open a serial terminal application as described above.

    2.  The application starts as a GAP central. To switch the role to a
GAP peripheral, press the  **ROLESW**. Depending on the role, when
pressing the  **SCANSW**, the application starts either scanning or
advertising.

    3.  As soon as the CONNLED turns solid on both devices, the user can
start writing in one of the consoles. The text appears on the other
terminal.
    4.  After creating a connection, the role (central or peripheral) is
displayed on the console. The role switch can be pressed again before
creating a new connection.

- When testing with a single board and the IoT Toolbox, perform the
following steps:

    1.  Open a serial port terminal and connect the board in the same
manner described above. The start screen is blank after the board is
reset.

    2.  Press the role switch button to behave as a GAP peripheral and
then press the **SCANSW** button to start advertising. The IoT Toolbox
app can then connect. Select UART instead of Console and start typing.