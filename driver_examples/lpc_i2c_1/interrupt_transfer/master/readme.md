# i2c_interrupt_transfer_master

## Overview
The i2c_interrupt_transfer_master example shows how to use i2c driver as master to do board to board transfer 
with interrupt:

In this example, one i2c instance as master and another i2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

