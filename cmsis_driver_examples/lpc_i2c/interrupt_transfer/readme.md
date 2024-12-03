# cmsis_i2c_interrupt_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_i2c_interrupt_transfer example shows how to use CMSIS I2C driver in interrupt way:

In this example , one i2c instance used as I2C master and another i2c instance used as I2C slave .
1. I2C master send data to I2C slave in interrupt . (I2C Slave using interrupt to receive the data)
2. I2C master read data from I2C slave in interrupt . (I2C Slave using interrupt to send the data)

