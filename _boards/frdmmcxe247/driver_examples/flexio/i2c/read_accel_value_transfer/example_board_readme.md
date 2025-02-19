Hardware requirements
===================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer

Board settings
============
The flexio_i2c_read_accel_value example is requires connecting the FLEXIO pins with the I2C pins of accelerometer
But this example no need special setting because of default FlexIO pins using is match with I2C pins of accelerometor
on board.

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value
Found an FXOS8700 on board, the device address is 0x1D .
The accel values:
status_reg = 0x81 , x =     7 , y =    11 , z =  1141
status_reg = 0x81 , x =    15 , y =     7 , z =  1133
status_reg = 0x81 , x =     3 , y =    15 , z =  1133
status_reg = 0x81 , x =     3 , y =     3 , z =  1155
status_reg = 0x81 , x =    -4 , y =    22 , z =  1163
status_reg = 0x81 , x =     0 , y =    15 , z =  1137
status_reg = 0x81 , x =    19 , y =    22 , z =  1125
status_reg = 0x81 , x =     0 , y =    26 , z =  1129
status_reg = 0x81 , x =    -8 , y =    15 , z =  1137
status_reg = 0x81 , x =     3 , y =    22 , z =  1144

End of FlexIO I2C example .
~~~~~~~~~~~~~~~~~~~~~
