Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
5.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

**Ensure that both the NCP FSCI Blackbox (for the NBU core) and the Wireless UART Host (for the Application core) have been flashed to the board.**

Running the demo
================
The application is built to work with another supported platform running either the Wireless UART or the Wireless UART Host example
or with the Wireless UART from the IoT Toolbox application.

When testing, the following steps must be performed:
1. Enter the “help” shell command to view a list of supported commands and their descriptions.
2. Enter the “database” shell command to create the dynamic GATT database and populate it. This is the mandatory first step.
3. Enter the “central” or “peripheral” shell commands to have the application start scanning as a GAP central device or advertising as a GAP peripheral device.
4. After the connection is established, the user can start writing using the “send” command. The messages will appear in the peer device’s console. Similarly, messages received will be displayed in the local console.
5. Optionally, the “disconnect” command can be used to disconnect from the peer device.
6. The “reset” command can be used to reset the device.
