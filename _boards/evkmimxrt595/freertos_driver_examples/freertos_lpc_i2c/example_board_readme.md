Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595
- Personal Computer

Board settings
==============
To make i2c example work, connections needed to be as follows:
        I2C11              connected to     I2C12
SCL     P4_21(J28 pin10)        -->         P5_1(J28 pin4)
SDA     P4_22(J28 pin9)        -->          P5_0(J28 pin3)

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J40) on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the demo.
