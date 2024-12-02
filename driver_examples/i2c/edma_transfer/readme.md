# i2c_edma

## Overview
The i2c_edma example shows how to use I2C driver in DMA way:

In this example , one i2c instance used as I2C master and another i2c instance used as I2C slave .
1. I2C master send data to I2C slave with DMA . (I2C Slave using interrupt to receive the data)
2. I2C master read data from I2C slave with DMA . (I2C Slave using interrupt to send the data)
