Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-EVK board
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

#### Please note this application can only support running with Real-Time Edge U-Boot ####
[Real-Time Edge Software](https://www.nxp.com/rtedge)
[Real-Time Edge U-Boot](https://github.com/nxp-real-time-edge-sw/real-time-edge-uboot)

Run the Demo
===============
1.  Connect 12V~20V power supply to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
    There will be 4 serial ports identified when connect USB cable to debug port.
    - The third serial port will be UART1, which is used by U-Boot and Linux.
    - The fourth serial port will be UART2 for FreeRTOS.

3.  Open 2 serial terminals connect to the above 2 ports with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Starting the FreeRTOS from the U-Boot command line:
    u-boot=> load mmc 1:2 0xD0000000 <path-to-image>/freertos_hello_ca55.bin;
    u-boot=> dcache flush;
    u-boot=> cpu 1 release 0xD0000000
5.  The FreeRTOS console will print out:
    'Hello world.'
