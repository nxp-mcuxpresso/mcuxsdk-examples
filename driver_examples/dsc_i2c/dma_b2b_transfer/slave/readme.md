# dsc_i2c_dma_b2b_transfer_slave

## Overview
The i2c_dma_b2b_transfer_slave example shows how to use i2c driver as slave to do board to board transfer 
with a DMA master in interrupt way:

In this example, one i2c instance as slave and another i2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
