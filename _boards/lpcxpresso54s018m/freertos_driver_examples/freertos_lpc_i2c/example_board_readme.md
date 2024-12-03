Hardware requirements
=====================
- Micro USB cable
- LPCXpresso54S018M board
- Personal Computer

Board settings
==============
To make i2c example work, connections needed to be as follows:
        I2C8              connected to     I2C9
SCL     P1_18(J9 pin2)        -->          P3_22(J9 pin11)
SDA     P1_17(J9 pin4)        -->          P3_21(J9 pin13)

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running
   the demo.
