# dsc_lpi2c_edma_b2b_transfer_slave

## Overview
The lpi2c_edma_b2b_transfer_slave example shows how to use lpi2c driver as slave to do board to board transfer 
with a EDMA master:

In this example, one lpi2c instance as slave and another lpi2c instance on the other board as master. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
