Hardware requirements
=====================
- Micro USB cable
- LPC54018-IoT-Module + Baseboard
- Personal Computer

Board settings
==============
To make i2c example work, connections needed to be as follows:
        I2C4              connected to     I2C2
SCL     P0_29(J27 pin2)        -->          P0_18(J30 pin6)
SDA     P0_30(J27 pin1)        -->          P0_17(J30 pin5)
Make sure JP42 2-3 is connected.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J5) on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running
   the demo.
