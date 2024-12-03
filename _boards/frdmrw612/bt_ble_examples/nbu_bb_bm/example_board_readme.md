Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board.
    use the USB port (J10) to communicate.
2.  The project set the 115200 baud rate as default, set flow as default, need modify the code, diabale the flow.
    The FRDM-RW612 doesn't suuport the flow transport and 3000000 baud rate.
    open the uart debugging tool, configure
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - no flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

