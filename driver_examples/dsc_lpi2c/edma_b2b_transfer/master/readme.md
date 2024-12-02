# dsc_lpi2c_edma_b2b_transfer_master

## Overview
The lpi2c_edma_b2b_transfer_master example shows how to use lpi2c driver as master to do board to board transfer 
with EDMA:

In this example, one lpi2c instance as master and another lpi2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
