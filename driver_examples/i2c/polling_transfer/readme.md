# i2c_polling_transfer

## Overview
The i2c_polling example shows how to use I2C driver in polling way:

In this example , one i2c instance used as I2C master and another i2c instance used as I2C slave .
1. I2C master send data to I2C slave in polling . (I2C Slave using interrupt to receive the data)
2. I2C master read data from I2C slave in polling . (I2C Slave using interrupt to send the data)

In this example, the values of three-axis accelerometer print to the serial terminal on PC through
the virtual serial port on board.
