Overview
========
This application implements a Bluetooh Low Energy HCI Black Box which can communicate over a serial interface.
To use the application Test Tool 12 or any other application which supports the HCI protocol is required.
For more information, please consult the "BLE Demo Applications User's Guide".

Toolchain supported
===================
- IAR Embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- kw47evk board

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
5.  Open Test Tool 12 and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
Use Test Tool 12 or any other application which supports the HCI protocol to interact with the device.
