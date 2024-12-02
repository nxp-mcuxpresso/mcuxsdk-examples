Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z board
- Personal Computer

Board settings
==============
The flexio_i2c_read_accel_value example is requires connecting the FLEXIO pins with the I2C pins of accelerometer
The connection should be set as following:
   FLEXIO_I2C        connected to  LPI2C0
SDA     PTA11(J2-17)     -->       PTA16(J2-18)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value

Found a FXOS8700 on board, the device address is 0x1C.

The accel values:

status_reg = 0xff , x =   511 , y =   -66 , z =  2065

status_reg = 0xff , x =   509 , y =   -70 , z =  2054

status_reg = 0xff , x =   513 , y =   -64 , z =  2064

status_reg = 0xff , x =   514 , y =   -71 , z =  2064

status_reg = 0xff , x =   508 , y =   -67 , z =  2066

status_reg = 0xff , x =   512 , y =   -62 , z =  2060

status_reg = 0xff , x =   514 , y =   -73 , z =  2060

status_reg = 0xff , x =   511 , y =   -64 , z =  2056

status_reg = 0xff , x =   518 , y =   -68 , z =  2064

status_reg = 0xff , x =   507 , y =   -67 , z =  2066

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~
