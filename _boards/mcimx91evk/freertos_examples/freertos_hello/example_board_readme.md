Hardware requirements
=====================
- USB Type-C cable
- MCIMX91-EVK board
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.


Run the Demo
===============
1.  Connect 12V~20V power supply to the board, switch SW1001 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB DBG port on the target board.
    There will be 4 serial ports identified when connect USB cable to debug port.
    - The third serial port will be UART1, which is used by U-Boot and Linux.

3.  Open a serial terminal connect to the above port with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Starting the FreeRTOS from the U-Boot command line:
    u-boot=> load mmc 1:2 0x80000000 <path-to-image>/freertos_hello.bin;
    u-boot=> dcache flush;
    u-boot=> go 0x80000000
5.  The FreeRTOS console will print out:
    'Hello world.'
