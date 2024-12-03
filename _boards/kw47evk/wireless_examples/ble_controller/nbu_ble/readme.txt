Overview
========
This application implements a Bluetooh Low Energy controller.
To use the application Test Tool 12 or any other application which supports the HCI protocol is required.
For more information, please consult the "BLE Demo Applications User's Guide".

Toolchain supported
===================
- IAR Embedded Workbench (ide version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- kw47evk board

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA/JLink USB port on the board.
2.  Download HCI black box application to the target board on core 0.
3.  Download the program via bl_host to the target board on core 1.
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
