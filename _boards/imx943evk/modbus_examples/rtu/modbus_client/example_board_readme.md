Hardware requirements
===================
- Mini/micro USB cable
- IMX943-EVK board
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Connect IMX943-EVK board and MIMXRT1180-EVK board with Dupont wire
        IMX943-EVK      J48-2(RX)----J39-4(TX)      MIMXRT1180-EVK
        IMX943-EVK      J48-4(TX)----J39-2(RX)      MIMXRT1180-EVK
        IMX943-EVK      J47-14(GND)---J39-3(GND)    MIMXRT1180-EVK
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the modbus_rtu_client program to IMX943-EVK board.
5.  Download the modbus_rtu_server program to MIMXRT1180-EVK board.
6.  Either re-power up your board or launch the debugger in your IDE to begin running the example.