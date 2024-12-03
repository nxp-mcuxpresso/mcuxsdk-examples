# i2c_read_fxls897x_accel_value_transfer

## Overview
The i2c_read_fxls897x_accel_value example shows how to use I2C driver to communicate with an i2c device:

 1. How to use the i2c driver to read a i2c device who_am_I register.
 2. How to use the i2c driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found an FXLS8974 on board, the device address is 0x18.
The accel values:
x =   -31 , y =   -39 , z =  1301
x =   -31 , y =   -39 , z =  1301
x =   -20 , y =   -23 , z =  1305
x =   -20 , y =   -23 , z =  1305
x =   -20 , y =   -23 , z =  1305
x =   -24 , y =   -12 , z =  1275
x =   -24 , y =   -12 , z =  1275
x =   -28 , y =   -27 , z =  1290
x =   -28 , y =   -27 , z =  1290
x =   -16 , y =   -20 , z =  1316

End of I2C example.

## Supported Boards
- [FRDM-MCXC041](../../../_boards/frdmmcxc041/driver_examples/i2c/read_fxls897x_accel_value_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/i2c/read_fxls897x_accel_value_transfer/example_board_readme.md)
- [FRDM-MCXC444](../../../_boards/frdmmcxc444/driver_examples/i2c/read_fxls897x_accel_value_transfer/example_board_readme.md)
