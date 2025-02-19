# lpi2c_read_fxls897x_accel_value_transfer

## Overview
The lpi2c_read_fxls897x_accel_value example shows how to use LPI2C driver to communicate with an i2c device:

 1. How to use the lpi2c driver to read a i2c device who_am_I register.
 2. How to use the lpi2c driver to write/read the device registers.

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPI2C example -- Read Accelerometer Value
Found an FXLS8974 on board, the device address is 0x18.
The accel values:
x =    18 , y =   -23 , z =  1050
x =    18 , y =   -23 , z =  1050
x =    14 , y =   -23 , z =  1044
x =    14 , y =   -23 , z =  1044
x =    14 , y =   -23 , z =  1044
x =     7 , y =   -12 , z =  1072
x =     7 , y =   -12 , z =  1072
x =    18 , y =   -35 , z =  1044
x =    18 , y =   -35 , z =  1044
x =    18 , y =   -35 , z =  1044

End of LPI2C example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXE247](../../../_boards/frdmmcxe247/driver_examples/lpi2c/read_fxls897x_accel_value_transfer/example_board_readme.md)
